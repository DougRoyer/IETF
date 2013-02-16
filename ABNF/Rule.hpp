/**
 * @file RuleName.hpp
 * This file defines the IETF ABNF parser Rule object.
 */

#ifndef _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_RULE_HPP_
#define _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_RULE_HPP_

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
#endif // WIN32

#include <string>		// Allow std::string
#include <wchar.h>		// Allow both (char*) and (wchar_t*) input.
#include <vector>

#ifdef BUILDING_LIBABNF_LIBRARY
#include "ABNF.hpp"
#include "RuleName.hpp"
#include "Terminal.hpp"
#include "Object.hpp"
#else // !BUILDING_LIBABNF_LIBRARY
#include <SaS/ABNF/ABNF.hpp>
#include <SaS/ABNF/RuleName.hpp>
#include <SaS/ABNF/Terminal.hpp>
#include <SaS/ABNF/Object.hpp>
#endif // BUILDING_LIBABNF_LIBRARY

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{

			// Forward REF
			//
			class Parser;

			/**
			 * @class RuleName
			 * An ABNF Rule name.
			 */
			class EXPORT_LIBABNF Rule
				: public RuleName,
				  public Object
			{

			public:

				/**
				 * Rule - Default Constructor.
				 */
				Rule(Parser & Parent);

				/**
				 * Rule - Destructor.
				 */
				virtual ~Rule();

				/**
				 * Parse a Rule.
				 *
				 * @param ParseAt A Pointer to where to start parsing at.
				 *
				 * @return true when parsed successfully, else returns false.
				 */
				virtual bool			Parse(char ** ParseAt);

				/**
				 * Get the ABNF as ASCII text.
				 *
				 * @param WithRepetitions When TRUE, print any repetition values.
				 *
				 * @return A string containing the ABNF in US-ASCII.
				 * Caller must free the returned value when they are done with it.
				 * Returns NULL when the Element name has not been set.
				 */
				virtual char		*	Print(bool WithRepetitions) const;

				/**
				 * @return true if this Object is a Terminal.
				 */
				virtual bool			IsTerminal() const;

				/**
				 * Get Elements.
				 */
				const std::vector<Object*> *	Elements() const;

			private:

				/**
				 * The parent ABNF object.
				 */
				Parser						*	_Parent;

				/**
				 * List of Rule and Terminal objects.
				 */
				std::vector<Object*>			_Elements;

			};
		}
	}
}

#endif // _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_RULE_HPP_