/**
 * @file ABNF.hpp
 * This file defines the IETF ABNF parser object.
 */

#ifndef _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_ABNF_HPP_
#define _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_ABNF_HPP_

#ifdef BUILDING_LIBABNF_LIBRARY
#include "Common.hpp"
#include "Rule.hpp"
#include "Terminal.hpp"
#else
#include <SaS/ABNF/Common.hpp>
#include <SaS/ABNF/Rule.hpp>
#include <SaS/ABNF/Terminal.hpp>
#endif // BUILDING_LIBABNF_LIBRARY

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{

			/**
			 * Errors. These can be thrown by any Parse() method.
			 */
			enum Error {
				/**
				 * new or malloc/calloc failed.
				 */
				NoMemory_e,

				/**
				 * Not a valid Terminal.
				 */
				 InvalidTerminal_e,

				/**
				 * Not a valid Rule name.
				 */
				 InvalidRuleName_e
			};

			/**
			 * Skip white space.
			 *
			 * @return The number of octets skipped.
			 */
			EXPORT_LIBABNF size_t				SkipWs(char ** ParseAt);

			/**
			 * Skip CRLF
			 *
			 * @return The number of octets skipped.
			 */
			EXPORT_LIBABNF size_t				SkipCRLF(char ** ParseAt);

			/**
			 * Get all characters until white space.
			 *
			 * @return The token name. Caller must release results when not needed.
			 */
			EXPORT_LIBABNF char				*	GetUntilWs(char ** ParseAt);

			/**
			 * Get the next sequence of characters that is a name.
			 *
			 * @return The name, or NULL if not a name. Caller must release results when not needed.
			 */
			EXPORT_LIBABNF char				*	GetName(char ** ParseAt);

			/**
			 * Look for the next sequence of characters that is a name.
			 *
			 * @return The name, or NULL if not a name. Caller must release results when not needed.
			 */
			EXPORT_LIBABNF char				*	LookName(char ** ParseAt);

			/**
			 * Get the next sequence of digits (0-9).
			 *
			 * @return The digits, or NULL if none. Caller must release results when not needed.
			 */
			EXPORT_LIBABNF char				*	GetDigits(char ** ParseAt);

			/**
			 * Look at all characters until white space.
			 *
			 * @return The token name. Caller must release results when not needed.
			 */
			EXPORT_LIBABNF char				*	LookUntilWs(char ** ParseAt);

			/**
			 * Determine if the provided character is within the range specified.
			 *
			 * @param Low The lowest acceptable value.
			 *
			 * @param High The highest acceptable value.
			 *
			 * @param ToTest The character to test.
			 *
			 * @return TRUE when: (ToTest >= Low) && (ToTest <= High).
			 */
			EXPORT_LIBABNF bool					InRange(unsigned char Low, unsigned char High, char ToTest);

			/**
			 * Determine if a string of zero or more characters matches a pattern.
			 *
			 * @param SetPattern The test pattern.
			 *
			 * @param TheString The string of zero or more characters to test.
			 *
			 * @return TRUE if TheString matches SetPattern.
			 */
			EXPORT_LIBABNF bool					IsPattern(char * SetPattern, const char * TheString);

			/**
			 * Determine if a string of zero or more characters matches a pattern.
			 *
			 * @param SetPattern The Terminal element to test against.
			 *
			 * @param TheString The string of zero or more characters to test.
			 *
			 * @return TRUE if TheString matches SetPattern.
			 */
			EXPORT_LIBABNF bool					IsPattern(Terminal * SetPattern, const char * TheString);

			/**
			 *  Move the parser ahead the specified number of bytes.
			 *
			 * @param ParseAt A pointer to the parser pointer.
			 *
			 * @param MoveAheadCount The number of bytes to move ahead.
			 *
			 * @return The next character pointed to.
			 * Returns early if (**ParseAt == 0) (end of string), at which point MoveForward will return zero (0).
			 */
			EXPORT_LIBABNF char					MoveForward(char ** ParseAt, size_t MoveAheadCount);

			/**
			 * The current Line Number. (first is 1).
			 */
			EXPORT_LIBABNF extern size_t		LineNumber;

			/**
			 * The current Character Position (first is 1).
			 */
			EXPORT_LIBABNF extern size_t		CharacterPosition;
		}
	}
}

#endif // _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_ABNF_HPP_

