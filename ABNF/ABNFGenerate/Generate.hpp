/**
 * @file Generate.hpp
 * This file describes the interface to generate code.
 *
 * @author Douglas Mark Royer <DouglasRoyer@gmail.com>
 */
#ifndef __SOFTWAREANDSERVICES_LIBRARY_IETF_ABNF_GENERATE_HPP__
#define __SOFTWAREANDSERVICES_LIBRARY_IETF_ABNF_GENERATE_HPP__

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
#include <ctype.h>

/**
 * The string "C++".
 */
extern const char			*	CPlusPlus;

/**
 * The string ".".
 */
extern const char			*	DirDot;

/** 
 * An empty string "".
 */
extern const char			*	Empty;

/**
 * The name of the ABNF input file.
 */
extern const char			*	AbnfFileName;

/**
 * When not empty, the name of the LIBRARY to build.
 */
extern const char			*	LibraryName;

/**
 * The selected computer language - as a string.
 * Defaults to C++
 */
extern const char			*	Language;

/**
 * The directory where all generated files will be created.
 * Default to "."
 */
extern const char			*	OutputDir;

/**
 * The namespace to use. Must be formatted
 * for the selected Language type.
 */
extern const char			*	Namespace;

/**
 * The name that ABNFGenerate programs.
 */
extern const char			*	ProgramName;

/**
 * When TRUE, progress messages will be displayed.
 */
extern bool						BeVerbose;

/**
 * When TRUE, an unrecoverable error has occurred.
 */
extern bool						FatalError;

/**
 * When non-NULL, the (FILE*) to the ABNF input file.
 */
extern FILE					*	AbnfInputFile;

/**
 * The entire ABNF file to parse in a buffer.
 */
extern char					*	FileDataToParse;

/**
 * Known Languages.
 */
enum Languages_t {

	/**
	 * Generate code using for the C++ programming language.
	 */
	CPlusPlus_l				= 1,

	/**
	 * Generate code using the ruby programming language (not yet supported).
	 */
	Ruby_l					= 2
};

/**
 * The currently selected language.
 */
extern Languages_t				SelectedLanguage;

/**
 * The current ABNFGenerate version.
 */
extern char					*	GenerateVersion;

using namespace SoftwareAndServices::Library::ABNF;

#ifdef MS_OS
/**
 * Get the file only part of a path.
 *
 * @param FileWithPath The filename with path (using '/' or '\').
 *
 * @return Returns the LAST component of FileWithPath, which is 
 * assumed to be the file name.
 */
extern const char			*	basename(const char * FileWithPath);
#endif

#endif // __SOFTWAREANDSERVICES_LIBRARY_IETF_ABNF_GENERATE_HPP__