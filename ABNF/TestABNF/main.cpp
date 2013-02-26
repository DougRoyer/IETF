
#define BUILDING_LIBABNF_LIBRARY

#include "../ABNF.hpp"
#include "../Parser.hpp"
#include "../Rule.hpp"
#include "../Terminal.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

using namespace SoftwareAndServices::Library::ABNF;

static const char						*	SimpleTestFile = "../abnf/SimpleTest.abnf";
static const char						*	SimpleUnResolved = "../abnf/SimpleUnResolved.abnf";
static const char						*	SimpleCharCase = "../abnf/SimpleCharCase.abnf";
static const char						*	SimpleMixed = "../abnf/SimpleMixed.abnf";
static const char						*	SimpleWrap = "../abnf/SimpleWrap.abnf";
static const char						*	SimpleOr = "../abnf/SimpleOr.abnf";
static const char						*	SimpleOption = "../abnf/SimpleOption.abnf";
static const char						*	SimpleGroup = "../abnf/SimpleGroup.abnf";
static const char						*	SimpleProse = "../abnf/SimpleProse.abnf";
static const char						*	SimpleRepeat = "../abnf/SimpleRepeat.abnf";
static const char						*	SimpleRange = "../abnf/SimpleRange.abnf";
static const char						*	SimpleSet = "../abnf/SimpleSet.abnf";

static const char						*	rfc4234 = "../abnf/rfc4234.abnf";

static const char						*	OutFile = "./ParsedOutput.abnf";
static const char						*	GoldFile = "../abnf/GoldOutput.abnf";

static int									TestCount = 0;
static int									ExitStatus = 0;

static FILE								*	aFile = NULL;
static FILE								*	GoldFp = NULL;
static FILE								*	OutFp = NULL;

static char								*	aBuffer = NULL;
static char								*	txt = NULL;

static Parser							*	TheParser = NULL;
static const std::vector<Rule*>			*	TheRules = NULL;
static Rule								*	aRule = NULL;
static std::vector<Rule*>::const_iterator	Rit;

void
	printErrorMessags(const std::vector<const char*> * ErrorMessages)
{

	std::vector<const char*>::const_iterator		it;
	const char									*	aValue = NULL;

	for (it = ErrorMessages->begin(); it != ErrorMessages->end(); it++) {
		aValue = *it;
		if (aValue != NULL) {
			fprintf(stderr, "%s\n", aValue);
		}
	}

	return;
}

void
	OpenAndGetFile(const char * FileName)
{
	aFile = fopen(FileName, "rb");

	if (aFile == NULL) {
		fprintf(stderr, "ERROR: Unable to open %s\n", FileName);
		ExitStatus++;
	
	} else {
		struct stat			sbuf;

		memset(&sbuf, '\0', sizeof(sbuf));
		fstat(aFile->_file, &sbuf);

		aBuffer = new char[sbuf.st_size + 1];
		aBuffer[sbuf.st_size] = '\0';

		fread(aBuffer, 1, sbuf.st_size, aFile);

		fprintf(stderr, "+++++++++++++++++++++++++++++++++++++++++++++++++++\nStarting: %s\n\n", FileName);
	}

	return;
}

void
	CleanUp()
{
	if (aBuffer != NULL) {
		delete aBuffer;
		aBuffer = NULL;
	}

	if (aFile != NULL) {
		fclose(aFile);
		aFile = NULL;
	}

	fprintf(stderr, "\nEnd\n+++++++++++++++++++++++++++++++++++++++++++++++++++\n\n");

	return;
}

int
	main(int argc, char ** argv)
{

	OutFp = fopen(OutFile, "wb");
	if (OutFile == NULL) {
		fprintf(stderr, "ERROR: Unable to open result file for writing: %s\n", OutFile);
		exit(1);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////
	//
	TestCount++;
	OpenAndGetFile(SimpleRepeat);

	TheParser = new Parser();

	TheParser->Text(aBuffer);
	if (!TheParser->Parse()) {
		fprintf(stderr, "Unable to parse: %s\n", SimpleRepeat);
		ExitStatus++;
	}

	txt = TheParser->Print();

	fprintf(OutFp, "%s\n",  txt);

	TheRules = TheParser->Rules();
	aRule = NULL;

	for (Rit = TheRules->begin(); Rit != TheRules->end(); Rit++) {
		aRule = *Rit;
		if (aRule != NULL) {
			if (!aRule->IsResolved) {
				fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values.\n", aRule->Name());
			}
		}
	}

	printErrorMessags(TheParser->Errors());

	delete TheParser;
	CleanUp();

	//////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////
	//
	TestCount++;
	OpenAndGetFile(SimpleTestFile);

	TheParser = new Parser();

	TheParser->Text(aBuffer);
	if (!TheParser->Parse()) {
		fprintf(stderr, "Unable to parse: %s\n", SimpleTestFile);
		ExitStatus++;
	}

	txt = TheParser->Print();

	fprintf(OutFp, "%s\n",  txt);

	TheRules = TheParser->Rules();
	aRule = NULL;

	for (Rit = TheRules->begin(); Rit != TheRules->end(); Rit++) {
		aRule = *Rit;
		if (aRule != NULL) {
			if (!aRule->IsResolved) {
				fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values - FAILURE.\n", aRule->Name());
				ExitStatus++;
			}
		}
	}
	printErrorMessags(TheParser->Errors());

	delete TheParser;
	CleanUp();

	//////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////
	//
	TestCount++;
	OpenAndGetFile(SimpleUnResolved);

	TheParser = new Parser();

	TheParser->Text(aBuffer);
	if (TheParser->Parse()) {
		fprintf(stderr, "%s parsed - And it should have had Rule3 unresolved.\n", SimpleUnResolved);
		ExitStatus++;
	}

	txt = TheParser->Print();

	fprintf(OutFp, "%s\n",  txt);

	TheRules = TheParser->Rules();
	aRule = NULL;

	for (Rit = TheRules->begin(); Rit != TheRules->end(); Rit++) {
		aRule = *Rit;
		if (aRule != NULL) {
			if (!aRule->IsResolved) {
				if (strcasecmp("Rule3", aRule->Name()) == 0) {
					fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values - PASS check for unresolved.\n", aRule->Name());
				
				} else if (strcasecmp("MainRule", aRule->Name()) == 0) {
					fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values - PASS check for unresolved (Rule3 was not defined).\n", aRule->Name());
				
				} else {
					fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values - FAILURE.\n", aRule->Name());
					ExitStatus++;
				}
			}
		}
	}
	printErrorMessags(TheParser->Errors());

	delete TheParser;
	CleanUp();

	//////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////
	//
	TestCount++;
	OpenAndGetFile(SimpleCharCase);

	TheParser = new Parser();

	TheParser->Text(aBuffer);
	if (!TheParser->Parse()) {
		fprintf(stderr, "Unable to parse: %s\n", SimpleCharCase);
		ExitStatus++;
	}

	txt = TheParser->Print();

	fprintf(OutFp, "%s\n",  txt);

	TheRules = TheParser->Rules();
	aRule = NULL;

	for (Rit = TheRules->begin(); Rit != TheRules->end(); Rit++) {
		aRule = *Rit;
		if (aRule != NULL) {
			if (!aRule->IsResolved) {
				fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values - FAILURE.\n", aRule->Name());
				ExitStatus++;
			}
		}
	}
	printErrorMessags(TheParser->Errors());

	delete TheParser;
	CleanUp();

	//////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////
	//
	TestCount++;
	OpenAndGetFile(SimpleMixed);

	TheParser = new Parser();

	TheParser->Text(aBuffer);
	if (!TheParser->Parse()) {
		fprintf(stderr, "Unable to parse: %s\n", SimpleMixed);
		ExitStatus++;
	}

	txt = TheParser->Print();

	fprintf(OutFp, "%s\n",  txt);

	TheRules = TheParser->Rules();
	aRule = NULL;

	for (Rit = TheRules->begin(); Rit != TheRules->end(); Rit++) {
		aRule = *Rit;
		if (aRule != NULL) {
			if (!aRule->IsResolved) {
				fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values - FAILURE.\n", aRule->Name());
				ExitStatus++;
			}
		}
	}
	printErrorMessags(TheParser->Errors());

	delete TheParser;
	CleanUp();

	//////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////
	//
	TestCount++;
	OpenAndGetFile(SimpleWrap);

	TheParser = new Parser();

	TheParser->Text(aBuffer);
	if (!TheParser->Parse()) {
		fprintf(stderr, "Unable to parse: %s\n", SimpleWrap);
		ExitStatus++;
	}

	txt = TheParser->Print();

	fprintf(OutFp, "%s\n",  txt);

	TheRules = TheParser->Rules();
	aRule = NULL;

	for (Rit = TheRules->begin(); Rit != TheRules->end(); Rit++) {
		aRule = *Rit;
		if (aRule != NULL) {
			if (!aRule->IsResolved) {
				fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values.\n", aRule->Name());
			}
		}
	}
	printErrorMessags(TheParser->Errors());

	delete TheParser;
	CleanUp();

	//////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////
	//
	TestCount++;
	OpenAndGetFile(SimpleOr);

	TheParser = new Parser();

	TheParser->Text(aBuffer);
	if (!TheParser->Parse()) {
		fprintf(stderr, "Unable to parse: %s\n", SimpleOr);
		ExitStatus++;
	}

	txt = TheParser->Print();

	fprintf(OutFp, "%s\n",  txt);

	TheRules = TheParser->Rules();
	aRule = NULL;

	for (Rit = TheRules->begin(); Rit != TheRules->end(); Rit++) {
		aRule = *Rit;
		if (aRule != NULL) {
			if (!aRule->IsResolved) {
				fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values.\n", aRule->Name());
			}
		}
	}
	printErrorMessags(TheParser->Errors());

	delete TheParser;
	CleanUp();

	//////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////
	//
	TestCount++;
	OpenAndGetFile(SimpleOption);

	TheParser = new Parser();

	TheParser->Text(aBuffer);
	if (!TheParser->Parse()) {
		fprintf(stderr, "Unable to parse: %s\n", SimpleOption);
		ExitStatus++;
	}

	txt = TheParser->Print();

	fprintf(OutFp, "%s\n",  txt);

	TheRules = TheParser->Rules();
	aRule = NULL;

	for (Rit = TheRules->begin(); Rit != TheRules->end(); Rit++) {
		aRule = *Rit;
		if (aRule != NULL) {
			if (!aRule->IsResolved) {
				fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values.\n", aRule->Name());
			}
		}
	}

	delete TheParser;
	CleanUp();

	//////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////
	//
	TestCount++;
	OpenAndGetFile(SimpleGroup);

	TheParser = new Parser();

	TheParser->Text(aBuffer);
	if (!TheParser->Parse()) {
		fprintf(stderr, "Unable to parse: %s\n", SimpleGroup);
		ExitStatus++;
	}

	txt = TheParser->Print();

	fprintf(OutFp, "%s\n",  txt);

	TheRules = TheParser->Rules();
	aRule = NULL;

	for (Rit = TheRules->begin(); Rit != TheRules->end(); Rit++) {
		aRule = *Rit;
		if (aRule != NULL) {
			if (!aRule->IsResolved) {
				fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values.\n", aRule->Name());
			}
		}
	}
	printErrorMessags(TheParser->Errors());

	delete TheParser;
	CleanUp();

	//////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////
	//
	TestCount++;
	OpenAndGetFile(SimpleProse);

	TheParser = new Parser();

	TheParser->Text(aBuffer);
	if (!TheParser->Parse()) {
		fprintf(stderr, "Unable to parse: %s\n", SimpleProse);
		ExitStatus++;
	}

	txt = TheParser->Print();

	fprintf(OutFp, "%s\n",  txt);

	TheRules = TheParser->Rules();
	aRule = NULL;

	for (Rit = TheRules->begin(); Rit != TheRules->end(); Rit++) {
		aRule = *Rit;
		if (aRule != NULL) {
			if (!aRule->IsResolved) {
				fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values.\n", aRule->Name());
			}
		}
	}

	delete TheParser;
	CleanUp();

	//////////////////////////////////////////////////////////////////////////////////////
	//
	TestCount++;
	OpenAndGetFile(SimpleProse);

	TheParser = new Parser();

	TheParser->Text(aBuffer);
	if (!TheParser->Parse()) {
		fprintf(stderr, "Unable to parse: %s\n", SimpleProse);
		ExitStatus++;
	}

	txt = TheParser->Print();

	fprintf(OutFp, "%s\n",  txt);

	TheRules = TheParser->Rules();
	aRule = NULL;

	for (Rit = TheRules->begin(); Rit != TheRules->end(); Rit++) {
		aRule = *Rit;
		if (aRule != NULL) {
			if (!aRule->IsResolved) {
				fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values.\n", aRule->Name());
			}
		}
	}

	delete TheParser;
	CleanUp();

	//////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////////////////
	//
	TestCount++;
	OpenAndGetFile(SimpleRange);

	TheParser = new Parser();

	TheParser->Text(aBuffer);
	if (!TheParser->Parse()) {
		fprintf(stderr, "Unable to parse: %s\n", SimpleRange);
		ExitStatus++;
	}

	txt = TheParser->Print();

	fprintf(OutFp, "%s\n",  txt);

	TheRules = TheParser->Rules();
	aRule = NULL;

	for (Rit = TheRules->begin(); Rit != TheRules->end(); Rit++) {
		aRule = *Rit;
		if (aRule != NULL) {
			if (!aRule->IsResolved) {
				fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values.\n", aRule->Name());
			}
		}
	}
	printErrorMessags(TheParser->Errors());

	delete TheParser;
	CleanUp();
   
	//////////////////////////////////////////////////////////////////////////////////////
	//
	TestCount++;
	OpenAndGetFile(SimpleSet);

	TheParser = new Parser();

	TheParser->Text(aBuffer);
	if (!TheParser->Parse()) {
		fprintf(stderr, "Unable to parse: %s\n", SimpleSet);
		ExitStatus++;
	}

	txt = TheParser->Print();

	fprintf(OutFp, "%s\n",  txt);

	TheRules = TheParser->Rules();
	aRule = NULL;

	for (Rit = TheRules->begin(); Rit != TheRules->end(); Rit++) {
		aRule = *Rit;
		if (aRule != NULL) {
			if (!aRule->IsResolved) {
				fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values.\n", aRule->Name());
			}
		}
	}
	printErrorMessags(TheParser->Errors());

	delete TheParser;
	CleanUp();
   
	//////////////////////////////////////////////////////////////////////////////////////
	//
	TestCount++;
	OpenAndGetFile(rfc4234);

	TheParser = new Parser();

	TheParser->Text(aBuffer);
	if (!TheParser->Parse()) {
		fprintf(stderr, "Unable to parse: %s\n", rfc4234);
		ExitStatus++;
	}

	txt = TheParser->Print();

	fprintf(OutFp, "%s\n",  txt);

	TheRules = TheParser->Rules();
	aRule = NULL;

	for (Rit = TheRules->begin(); Rit != TheRules->end(); Rit++) {
		aRule = *Rit;
		if (aRule != NULL) {
			if (!aRule->IsResolved) {
				fprintf(stderr, "Rule: %s: Does not fully resolve to Terminal values.\n", aRule->Name());
			}
		}
	}
	printErrorMessags(TheParser->Errors());

	delete TheParser;
	CleanUp();
   
	//////////////////////////////////////////////////////////////////////////////////////
	// 

	if (ExitStatus == 0) {
		fprintf(stderr, "ALL PASS (%d/%d)\n", TestCount, TestCount);
	} else {
		fprintf(stderr, "FAILED %d of %d passed (%d failed)\n", TestCount - ExitStatus, TestCount, ExitStatus);
	}

	//////////////////////////////////////////////////////////////////////////////////////

	GoldFp = fopen(GoldFile, "rb");
	if (GoldFp == NULL) {
		fprintf(stderr, "ERROR: Unable to open GOLD result file: %s\n", GoldFile);
	} else {

		fclose(OutFp);					// Close the file for writing.
		OutFp = fopen(OutFile, "rb");	// Open the same file for reading.
		if (OutFile == NULL) {
			fprintf(stderr, "ERROR: Unable to open result file for reading: %s\n", OutFile);
		} else {
	

			struct stat			OutStatBuf;
			struct stat			GoldStatBuf;

			char			*	OutBuf = NULL;
			char			*	GoldBuf = NULL;

			memset(&OutStatBuf, '\0', sizeof(OutStatBuf));
			fstat(OutFp->_file, &OutStatBuf);
			OutBuf = new char[OutStatBuf.st_size + 1];
			OutBuf[OutStatBuf.st_size] = '\0';
			fread(OutBuf, 1, OutStatBuf.st_size, OutFp);

			memset(&GoldStatBuf, '\0', sizeof(GoldStatBuf));
			fstat(GoldFp->_file, &GoldStatBuf);
			GoldBuf = new char[GoldStatBuf.st_size + 1];
			GoldBuf[GoldStatBuf.st_size] = '\0';
			fread(GoldBuf, 1, GoldStatBuf.st_size, GoldFp);

			if (OutStatBuf.st_size != GoldStatBuf.st_size) {
				fprintf(stderr, "ERROR: Output and Gold file sizes do not match.\n");
			}

			if (strncmp(GoldBuf, OutBuf, GoldStatBuf.st_size) != 0) {
				fprintf(stderr, "ERROR: Output and Gold files do not match.\n");
			}
			//////////////////////////////////////////////////////////////////////////////////////

			fclose(GoldFp);
			fclose(OutFp);
		}
	}

	return(ExitStatus);
}
