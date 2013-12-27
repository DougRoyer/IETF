/**
 * @file Comment.hpp
 * This file defines the IETF ABNF Comment object.
 *
 * @author Douglas Mark Royer <DouglasRoyer@gmail.com>
 */

#ifndef _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_COMMENT_HPP_
#define _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_COMMENT_HPP_

#ifdef BUILDING_LIBABNF_LIBRARY
#include "Common.hpp"
#include "RuleName.hpp"
#include "Object.hpp"
#else
#include <SaS/IETF/ABNF/Common.hpp>
#include <SaS/IETF/ABNF/RuleName.hpp>
#include <SaS/IETF/ABNF/Object.hpp>
#endif // BUILDING_LIBABNF_LIBRARY

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{

			/**
			 * @class Comment
			 * An ABNF Comment.
			 */
			class EXPORT_LIBABNF Comment
				: public Object
			{

			public:

				/**
				 * Comment - Default Constructor.
				 */
				Comment();

				/**
				 * Terminal - Destructor.
				 */
				virtual ~Comment();

				/**
				 * Parse a Terminal.
				 *
				 * @param ParseAt A Pointer to where to start parsing at.
				 *
				 * @return true when successfully parsed, else returns false.
				 */
				virtual bool		Parse(char ** ParseAt);

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

			private:

				/**
				 * The entire comment value (minus the ';').
				 */
				char			*	_Comment;
			};
		}
	}
}

#endif // _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_COMMENT_HPP_

