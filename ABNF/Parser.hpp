/**
 * @file ABNF.hpp
 * This file defines the IETF ABNF parser object.
 */

#ifndef _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_PARSER_HPP_
#define _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_PARSER_HPP_

#include <SaS/ABNF/Common.hpp>

#include <wchar.h>		// Allow both (char*) and (wchar_t*) input.

#include "Rule.hpp"
#include "Terminal.hpp"

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{

			/**
			 * @class Parser
			 * Parse IETF ABNF (RFC4234).
			 * See <a href="http://ietf.org/rfc/rfc4234.txt">RFC-4234</a>.
			 *
			 * The (wchar_t*) methods are defined because a popular compiler uses
			 * UTF-16 by default for it runtime libraries.
			 *
			 * There are Rule and Element objects.
			 * The topmost Rule is fetched by calling Top().
			 *
			 * @see Rule
			 * @see Terminal
			 * @see Object
			 */
			class EXPORT_LIBABNF Parser
			{

			public:

				/**
				 * Parser - Default Constructor.
				 */
				Parser();

				/**
				 * Parser - Destructor.
				 */
				virtual ~Parser();

				/**
				 * Set the text to be parsed.
				 *
				 * @param InputText The text to parse. This simply initialized the Parse
				 * with the text to parse, no parsing is done until the Start() method is called.
				 * InputText is assumed to be in US-ASCII as that is how IETF RFC's are published.
				 *
				 * If the InputText was previously set, the last call overrides any previous calls
				 * or constructor setup of the InputText.
				 */
				void							Text(char * InputText);

				/**
				 * Set the text to be parsed.
				 *
				 * @param InputText The text to parse. This simply initialized the Parse
				 * with the text to parse, no parsing is done until the Start() method is called.
				 * InputText is assumed to be in US-ASCII as that is how IETF RFC's are published.
				 *
				 * If the InputText was previously set, the last call overrides any previous calls
				 * or constructor setup of the InputText.
				 */
				void							Text(wchar_t * InputText);

				/**
				 * Parse the input text.
				 *
				 * @return Parse() will return true when everything was successfully parse.
				 * Returns false when the input text was not parsed successfully, or if the input
				 * text was not set.
				 */
				bool							Parse();

				/**
				 * Get the topmost Rule for the parsed ABNF.
				 *
				 * @return The topmost Rule or NULL if the ABNF was not parsed successfully.
				 */
				Rule						*	Top() const;

				/**
				 * Get the ABNF as ASCII text.
				 *
				 * @param ParseAt A Pointer to where to start parsing at.
				 *
				 * @return A string containing the ABNF in US-ASCII.
				 * Caller must free the returned value when they are done with it.
				 */
				char						*	Print() const;

				/**
				 * Add a new Rule if new, or get the pointer to the rule if it already exists.
				 *
				 * @param TheRuleName The Rule to look for, or add as new.
				 *
				 * @return  The new Rule, or a pointer to the existing one with TheRuleName.
				 */
				Rule						*	Add(char * TheRuleName);

				/**
				 * Add an existing Rule to the list.
				 * Does not add TheRule if it already exists in the list.
				 *
				 * @param TheRule A Rule to add to the list.
				 *
				 * @return  TheRule
				 */
				Rule						*	Add(Rule * TheRule);

				/**
				 * Get the parsed rules.
				 *
				 * @return a const std::vector<Rule*> of all Rule objects.
				 */
				const std::vector<Rule*>	*	Rules() const;

			private:

				/**
				 * Determine if the named Rule resolves to only terminals.
				 *
				 * @param TheRule The named Rule to check.
				 *
				 * @return true when all paths lead to a terminal.
				 */
				bool							_Resolved(Rule * TheRule);

				/**
				 * The topmost Rule.
				 */
				Rule				*	_Topmost;

				/**
				 * List of rules.
				 */
				std::vector<Rule*>		_Rules;

				/**
				 * Start of parsing text.
				 */
				char				*	_ParseAt;

				/**
				 * True when we allocated _ParseAt.
				 */
				bool					_OurParseAt;
			};
		}
	}
}

#endif // _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_PARSER_HPP_

