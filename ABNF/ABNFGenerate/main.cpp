/**
 * @file main.cpp
 * This file describes the interface to ABNFGenerate
 *
 * @author Douglas Mark Royer <DouglasRoyer@gmail.com>
 */
#include "../libABNF/ABNF.hpp"
#include "../libABNF/Comment.hpp"
#include "../libABNF/Common.hpp"
#include "../libABNF/Object.hpp"
#include "../libABNF/Parser.hpp"
#include "../libABNF/Rule.hpp"
#include "../libABNF/RuleName.hpp"
#include "../libABNF/Terminal.hpp"
#include "getopt.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <direct.h>

#include "Generate.hpp"

const char					*	CPlusPlus = "C++";
const char					*	DirDot = ".";
const char					*	Empty = "";

const char					*	AbnfFileName = NULL;
const char					*	Language = CPlusPlus;
const char					*	LibraryName = Empty;
const char					*	OutputDir = DirDot;
const char					*	Namespace = Empty;
const char					*	ProgramName = NULL;

char						*	FileDataToParse = NULL;

bool							BeVerbose = false;
bool							FatalError = false;

FILE						*	AbnfInputFile = NULL;

extern int						Generate();

Languages_t						SelectedLanguage = CPlusPlus_l;

char						*	GenerateVersion = GENERATE_VERSION;

#ifdef MS_OS
const char	*
	basename(const char * FileWithPath)
{
	const char			*	Results = strrchr(FileWithPath, '/');

	if (Results != NULL) {
		Results++;
	} else {
		Results = strrchr(FileWithPath, '\\');

		if (Results == NULL) {
			Results = FileWithPath;
		} else {
			Results++;
		}
	}

	return(Results);
}

#endif // MS_OS
void
	usage(const char * argv0)
{
	static const char	*	Usage =
		"usage: %s --abnf=<input-file> [other-options]\n\n"
		"-a, --abnf=<input-file>		Required. The name of the ABNF input file.\n"
		"				Use '-' for stdin.\n\n"

		"-b, --build-library=<library-name>	Optional. When supplied, the name of\n"
		"				the library to build.\n\n"

		"-l, --language=C++		Optional. The output will be compliable in\n"
		"				this language. (C++ is the default)\n\n"

		"-o, --output-dir=<out-dir>	Optional. If supplied the generated files will\n"
		"				be in <out-dir>. It will be created if needed.\n\n"

		"-n, --namespace=<useNamespace>	Optional. If supplied the namespace to use.\n"
		"				Must be supplied in format acceptable to \n"
		"				--language value.\n\n"

		"-v, --verbose			Optional. Be verbose about progress.\n\n"

		"-h, --help			Print this message.\n\n";

	fprintf(stderr, Usage, basename(argv0));

	return;			
}

extern "C" {
	void
		warnx(const char * fmt, ...)
	{
		char			*	Buffer = NULL;
		size_t				Len = 0;
		size_t				Offset = strlen(ProgramName) + 2;

		va_list				args;

		va_start(args, fmt);
		Len = _vscprintf(fmt, args) + 1;

		Buffer = new char[Len + strlen(ProgramName) + 20];

		strcpy(Buffer, ProgramName);
		strcat(Buffer, ": ");
		vsprintf(&Buffer[Offset], fmt, args);

		fprintf(stderr, Buffer);
		fprintf(stderr, "\n");

		delete Buffer;

		return;
	}
}

int
	main(int argc, char ** argv)
{
	int					ExitStatus = 1;

	char				FoundOpt;
	int					digit_optind = 0;

	ProgramName = argv[0];

	while (1) {
		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{"abnf",			required_argument,	0,	'a' },
			{"language",		required_argument,	0,  'l' },
			{"output-dir",		required_argument,	0,  'o' },
			{"build-library",	required_argument,	0,  'b' },
			{"namespace",		required_argument,	0,	'n' },
			{"verbose",			no_argument,		0,  'v' },
			{"help",			no_argument,		0,	'h' },
			{0,				0,					0,  0 }
		};

		FoundOpt = getopt_long(argc, argv, "a:b:l:o:n:v",
								long_options, 
								&option_index);
		
		if (FoundOpt == -1) {
			break;
		}

		switch (FoundOpt) {

		case 'a':
			AbnfFileName = optarg;
			break;

		case 'b':
			LibraryName = optarg;
			break;

		case 'l':
			Language = optarg;
			break;

		case 'o':
			OutputDir = optarg;
			break;

		case 'n':
			Namespace = optarg;
			break;

		case 'v':
			BeVerbose = true;
			break;

		case '?':
			usage(argv[0]);
			break;

		default:
			usage(argv[0]);
			break;
		}
	}

	if (AbnfFileName == NULL ) {
		usage(argv[0]);
		FatalError = true;
	} else {

		if (BeVerbose) {
			fprintf(stderr, "Opening and parsing ABNF file: %s\n", AbnfFileName);
		}

		AbnfInputFile = fopen(AbnfFileName, "rb");

		if (AbnfInputFile == NULL) {
			fprintf(stderr, "ERROR: Unable to open ABNF input file for reading: %s\n", AbnfFileName);
			FatalError = true;
		}
	}

	if (OutputDir != DirDot && strcmp(OutputDir, DirDot) != 0) {
		if (BeVerbose) {
			fprintf(stdout, "Testing for and making if needed, directory: %s\n", OutputDir);
		}
#ifdef MS_OS
		_mkdir(OutputDir);
#else
		mkdir(OutPutDir);
#endif
		struct stat			StatBuf;

		memset(&StatBuf, '\0', sizeof(StatBuf));

		if (stat(OutputDir, &StatBuf) != 0) {
			fprintf(stderr, "ERROR: Unable to create or access Output Directory (%s): %s\n", strerror(errno), OutputDir);
			FatalError = true;
		}
	}

	if (Language != NULL) {
		if (strcmp(Language, CPlusPlus) != 0) {
			fprintf(stderr, "ERROR: Not supported Language: %s\n", Language);
			FatalError = true;
		} else {
			SelectedLanguage = CPlusPlus_l;
		}

		if (!FatalError && BeVerbose) {
			fprintf(stderr, "Building files for language: %s\n", Language);
		}
	}

	if (Namespace != Empty && BeVerbose) {
		fprintf(stderr, "All %s objects will be in the namespace: %s\n", Language, Namespace);
	}

	if (!FatalError) {
		struct stat			ParseStat;

		if (stat(AbnfFileName, &ParseStat) == 0) {
			FileDataToParse = new char[ParseStat.st_size] + 1;

			fread((void*)FileDataToParse, sizeof(char), ParseStat.st_size, AbnfInputFile);
			FileDataToParse[ParseStat.st_size] = '\0';

			ExitStatus = Generate();

		} else {
			fprintf(stderr, "ERROR: Can not open ABNF file for reading: %s\n", AbnfFileName);
			FatalError = true;
		}
		
	}

	if (FatalError) {
		ExitStatus = 1;
	}

	return(ExitStatus);
}
