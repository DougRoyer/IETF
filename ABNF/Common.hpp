/**
 * @file ABNF.hpp
 * This file defines the IETF ABNF parser object.
 */

#ifndef _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_COMMON_HPP_
#define _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_COMMON_HPP_

#ifdef WIN32

#ifdef EXPORT_OUT
#undef EXPORT_OUT
#endif // EXPORT_OUT
#define EXPORT_OUT __declspec(dllexport)

#ifdef EXPORT_IN
#undef EXPORT_IN
#endif // EXPORT_IN
#define EXPORT_IN __declspec(dllimport)

#ifndef EXPORT_LIBABNF

#ifdef BUILDING_LIBABNF_LIBRARY
#define EXPORT_LIBABNF EXPORT_OUT
#else // !BUILDING_LIBABNF_LIBRARY
#define EXPORT_LIBABNF EXPORT_IN
#endif // BUILDING_LIBABNF_LIBRARY

#endif // EXPORT_LIBABNF
#else // !WIN32
#define EXPORT_OUT
#define EXPORT_IN
#define EXPORT_LIBABNF
#endif // WIN32

#include <wchar.h>		// Allow both (char*) and (wchar_t*) input.
#include <vector>

#ifdef WIN32
#define strcasecmp(a,b)	stricmp(a,b)
#endif // WIN32

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{

		}
	}
}

#endif // _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_COMMON_HPP_

