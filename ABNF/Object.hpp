/**
 * @file GenericObject.hpp
 * This file defines a base object for Rule and Terminal objects.
 */

#ifndef _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_OBJECT_HPP_
#define _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_OBJECT_HPP_

#ifdef BUILDING_LIBABNF_LIBRARY
#include "Common.hpp"
#else
#include <SaS/ABNF/Common.hpp>
#endif // BUILDING_LIBABNF_LIBRARY

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{

			/**
			 * @class Object
			 * An ABNF Object that is the base for a Rule or Terminal.
			 */
			class EXPORT_LIBABNF Object
			{

			public:

				enum DataType {

					/**
					 * Is a logical AND.
					 * This is the default:	foo bar
					 */
					IsAnd,

					/**
					 * Is a logical OR: foo / bar
					 */
					IsOr,

					/**
					 * Is a logical GROUP: a (foo / bar)
					 */
					IsGroup,

					/**
					 * Is a Range of values. %d13-20
					 */
					IsRange,

					/**
					 * Is Set of values. %d13.10.9
					 */
					IsSet,

					/**
					 * Is Group () start (
					 */
					GroupStart,

					/**
					 * Is Group () end )
					 */
					GroupEnd,

					/**
					 * Is Option [] start [
					 */
					OptionStart,

					/**
					 * Is Option [] end ]
					 */
					OptionEnd,

					/**
					 * Is Prose <> start <
					 */
					ProseStart,

					/**
					 * Is Prose <> end >
					 */
					ProseEnd,

					/**
					 * Is a String "xxx"
					 */
					IsString,

					/**
					 * Is binary data %b...
					 */
					IsBinary,

					/**
					 * Is hexadecimal data %x...
					 */
					IsHexadecimal,

					/**
					 * Is Decimal data %d...
					 */
					IsDecimal,

					/**
					 * Is comment.
					 */
					IsComment
				};

				/**
				 * Object - Default Constructor.
				 */
				Object();

				/**
				 * Parse a Object.
				 *
				 * @param ParseAt A Pointer to where to start parsing at.
				 *
				 * @return true when successfully parsed, else returns false.
				 */
				virtual bool				Parse(char ** ParseAt) = 0;

				/**
				 * Get the ABNF as ASCII text.
				 *
				 * @param WithRepetitions When TRUE, print any repetition values.
				 *
				 * @return A string containing the ABNF in US-ASCII.
				 * Caller must free the returned value when they are done with it.
				 * Returns NULL when the Element name has not been set.
				 */
				virtual char	*	Print(bool WithRepetitions) const = 0;

				/**
				 * @return true if this Object is a Terminal.
				 */
				virtual bool				IsTerminal() const = 0;

				/**
				 * True when the code is checking this object for resolution.
				 */
				bool						IsBeingResolved;

				/**
				 * True when this object is resolved to a terminal.
				 * Always true for Terminal objects.
				 */
				bool						IsResolved;

				/**
				 * True when this object is an element, and not a rule definition.
				 */
				bool						IsElement;

				/**
				 * Parse a repetition rule.
				 *
				 * @param ParseAt A pointer to the data to parse.
				 *
				 * @return The TRUE if there was a repetition rule, else FALSE.
				 * FALSE means 1*1
				 */
				bool						ParseRepetition(char ** ParseAt);

				/**
				 * Parse a repetition rule.
				 *
				 * @param ParseAt A pointer to the data to parse.
				 *
				 * @param Min A reference to the minimum value after the parse.
				 * Only valid if ParseRepetition() returns NON-NULL.
				 *
				 * @param Max A reference to the maximum value after the parse.
				 * Only valid if ParseRepetition() returns NON-NULL.
				 *
				 * @return The TRUE if there was a repetition rule, else FALSE.
				 * FALSE means 1*1
				 */
				static bool					ParseRepetition(char ** ParseAt, size_t & min, size_t & max);

				/**
				 * Minimum occurs.
				 */
				unsigned int				Min;

				/**
				 * Maximum occurs.
				 */
				unsigned int				Max;

			protected:

				/**
				 * Print variable repetition.
				 *
				 * @return Any variable repetition, Or NULL if none. Caller must free results.
				 */
				char					*	_PrintRepetition() const;

				/**
				 * What type of data is this.
				 */
				DataType					_DataType;

			};
		}
	}
}

#endif // _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_OBJECT_HPP_

