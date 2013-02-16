/**
 * @file RuleName.hpp
 * This file defines the IETF ABNF parser Comment object.
 */

#ifndef _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_COMMENT_HPP_
#define _DOUGLAS_MARK_ROYER_LIBRARY_LIBABNF_COMMENT_HPP_

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

#include <wchar.h>		// Allow both (char*) and (wchar_t*) input.
#include <vector>

#ifdef BUILDING_LIBABNF_LIBRARY
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