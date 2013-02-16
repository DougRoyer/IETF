/**
 * @file RuleName.hpp
 * This file defines the IETF ABNF parser Terminal object.
 */

#ifndef _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_TERMINAL_HPP_
#define _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_TERMINAL_HPP_

#include <SaS/ABNF/Common.hpp>

#include <wchar.h>		// Allow both (char*) and (wchar_t*) input.
#include <vector>

#ifdef BUILDING_LIBABNF_LIBRARY
#include "RuleName.hpp"
#include "Object.hpp"
#else // !BUILDING_LIBABNF_LIBRARY
#include <SaS/ABNF/RuleName.hpp>
#include <SaS/ABNF/Object.hpp>
#endif // BUILDING_LIBABNF_LIBRARY

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{

			/**
			 * @class Terminal
			 * An ABNF Terminal.
			 */
			class EXPORT_LIBABNF Terminal
				: public Object,
				public std::vector<char*>	// The values.
			{

			public:

				/**
				 * Terminal - Default Constructor.
				 */
				Terminal();

				/**
				 * Terminal - Destructor.
				 */
				virtual ~Terminal();

				/**
				 * Parse a Terminal.
				 *
				 * @param ParseAt A Pointer to where to start parsing at.
				 *
				 * @return true when successfully parsed, else returns false.
				 */
				virtual bool			Parse(char ** ParseAt);

				/**
				 * Parse a Binary Value.
				 *
				 * @return The binary value as an ABNF string.
				 */
				char				*	ParseBinary(char ** ParseAt);

				/**
				 * Parse a Decimal Value.
				 *
				 * @return The decimal value as an ABNF string.
				 */
				char				*	ParseDecimal(char ** ParseAt);

				/**
				 * Parse a Hexadecimal value.
				 *
				 * @return The hexadecimal vale as an ABNF string.
				 */
				char				*	ParseHexadecimal(char ** ParseIn);

				/**
				 * Get the ABNF as ASCII text.
				 *
				 * @param WithRepetitions When TRUE, print any repetition values.
				 *
				 * @return A string containing the ABNF in US-ASCII.
				 * Caller must free the returned value when they are done with it.
				 * Returns NULL when the Element name has not been set.
				 */
				virtual char	*	Print(bool WithRepetitions) const;

				/**
				 * @return true if this Object is a Terminal.
				 */
				virtual bool		IsTerminal() const;

			};
		}
	}
}

#endif // _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_TERMINAL_HPP_

