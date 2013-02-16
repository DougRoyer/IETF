/**
 * @file RuleName.hpp
 * This file defines the IETF ABNF parser Rule 'name' object.
 */

#ifndef _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_RULE_NAME_HPP_
#define _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_RULE_NAME_HPP_

#include <SaS/ABNF/Common.hpp>

#include <wchar.h>		// Allow both (char*) and (wchar_t*) input.

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{

			/**
			 * A list of characters that can be the first letter of a rule name.
			 */
			EXPORT_LIBABNF extern const char				*	RuleNameStart;

			/**
			 * A list of characters that can be the first letter of a rule name.
			 */
			EXPORT_LIBABNF extern const wchar_t				*	WRuleNameStart;

			/**
			 * A list of characters that can follow the first letter of a rule name.
			 */
			EXPORT_LIBABNF extern const char				*	RuleNameCharacter;

			/**
			 * A list of characters that can follow the first letter of a rule name.
			 */
			EXPORT_LIBABNF extern const wchar_t				*	WRuleNameCharacter;


			/**
			 * @class RuleName
			 * An ABNF Rule name.
			 */
			class EXPORT_LIBABNF RuleName {

			public:

				/**
				 * RuleName exceptions.
				 */
				enum NameException {
					/**
					 * The rule name is not valid.
					 * Rule names are described in RFC4234 section 2.1 as
					 * ... a sequence of characters, beginning with an alphabetic character,
					 * followed by a combination of alphabetics, digits, and hyphens ...
					 */
					NotAValidRuleName_e		= 1,	

					/**
					 * The system failed to allocate more memory to process your request.
					 */
					OutOfMemory_e			= 2
				};

				/**
				 * RuleName - Default Constructor.
				 */
				RuleName();

				/**
				 * RuleName - Destructor.
				 */
				virtual ~RuleName();

				/**
				 * @return This objects rule name, or NULL if not set.
				 */
				const char			*	Name() const;

				/**
				 * @param TheRuleName The rule name to use.
				 *
				 * @return false when TheRuleName is not a valid rule name.
				 */
				bool					Name(char * TheRuleName);

				/**
				 * @param TheRuleName The rule name to use.
				 *
				 * @return false when TheRuleName is not a valid rule name.
				 */
				bool					Name(wchar_t * TheRuleName);

				/**
				 * Determine if what is passed in is a valid rule name.
				 * Checks the name up until the end of the string or until
				 * the first space, carriage return, or newline is found.
				 *
				 * @param ValidNameCharacters Optional. When not null, it is a pointer
				 * to a size_t variable that will contain the number of valid rule
				 * name characters.
				 *
				 * @return true if the string passed in starts with a valid rule name.
				 */
				bool					IsName(char * NameToCheck, size_t * ValidNameCharacters = NULL) const;

				/**
				 * Determine if what is passed in is a valid rule name.
				 * Checks the name up until the end of the string or until
				 * the first space, carriage return, or newline is found.
				 * <b>NOTE: Per RFC4234, names are case insensitive.</b>
				 *
				 * @param ValidNameCharacters Optional. When not null, it is a pointer
				 * to a size_t variable that will contain the number of valid rule
				 * name characters.
				 *
				 * @return true if the string passed in starts with a valid rule name.
				 */
				bool					IsName(wchar_t * NameToCheck, size_t * ValidNameCharacters = NULL) const;

				/**
				 * Parse a RuleName.
				 *
				 * @param ParseAt A Pointer to where to start parsing at.
				 *
				 * @return true when parsed successfully, else returns false.
				 */
				bool				Parse(char ** ParseAt);

				/**
				 * Get the ABNF as ASCII text.
				 *
				 * @return A string containing the ABNF in US-ASCII.
				 * Caller must free the returned value when they are done with it.
				 * Returns NULL when the RuleName has not been set.
				 */
				char			*	Print() const;

			private:

				/**
				 * The Rule name.
				 */
				char				*	_RuleName;
			};
		}
	}
}

#endif // _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_RULE_NAME_HPP_

