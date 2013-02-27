// This is the main DLL file.

#include <ctype.h>
#include <string.h>
#include "ABNF.hpp"

#ifdef WIN32

/**
 * A macro that defines strcasecmp.
 *
 * @author	DouglasRoyer@gmail.com
 * @date	11 Feb 2013
 *
 * @param	a	The void to process.
 * @param	b	The void to process.
 */
#define strcasecmp(a,b)	stricmp(a,b)
#endif // WIN32

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{
			
			static const char	*	ValidInName = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-";

			EXPORT_OUT size_t		LineNumber = 1;

			EXPORT_OUT size_t		CharacterPosition = 1;

			EXPORT_OUT size_t
				SkipWs(char ** ParseAt)
			{
				size_t						Results = 0;

				if (ParseAt != NULL && *ParseAt != NULL) {
					if (**ParseAt != '\0') {

						while (**ParseAt == ' ' || **ParseAt == '\t') {
							Results++;
							MoveForward(ParseAt, 1);
						}
					}
				}

				return(Results);
			}

			EXPORT_OUT size_t
				SkipCRLF(char ** ParseAt)
			{
				size_t						Results = 0;

				if (ParseAt != NULL && *ParseAt != NULL) {
					if (**ParseAt != '\0') {
						if (**ParseAt == '\r' && *(*(ParseAt) + 1) == '\n') {

							/**
							 * .
							 */
							Results += 2;
							MoveForward(ParseAt, 2);
						}
					///< .
					}
				}

				return(Results);
			}

			EXPORT_OUT char	*
				GetUntilWs(char ** ParseAt)
			{
				char				*	Results = NULL;

				if (ParseAt != NULL && *ParseAt != NULL) {
					const char		*	StartAt = *ParseAt;
					const char		*	Ptr = *ParseAt;
					char				aChar = *Ptr;

					while (**ParseAt != '\0' && !isspace(**ParseAt)) {
						MoveForward(ParseAt, 1);
					}

					if (*ParseAt != StartAt) {
						size_t			Len = *ParseAt - StartAt;

						Results = new char[Len + 1];

						strncpy(Results, StartAt, Len);
						Results[Len] = '\0';
					}
				}
				
				return(Results);
			}

			EXPORT_OUT char	*
				LookUntilWs(char ** ParseAt)
			{
				char				*	Results = NULL;

				if (ParseAt != NULL && *ParseAt != NULL) {
					const char		*	LookAt = *ParseAt;
					const char		*	StartAt = LookAt;
					const char		*	Ptr = LookAt;
					char				aChar = *Ptr;

					while (*LookAt != '\0' && !isspace(*LookAt)) {
						LookAt++;
					}

					if (LookAt != StartAt) {
						size_t			Len = LookAt - StartAt;

						///< An enum constant representing the results option
						Results = new char[Len + 1];

						strncpy(Results, StartAt, Len);
						Results[Len] = '\0';
					}
				}

				return(Results);
			}

			EXPORT_OUT bool
				InRange(unsigned char Low, unsigned char High, char ToTest)
			{
				return(((ToTest >= Low) && (ToTest <= High)));
			}

			EXPORT_OUT bool
				IsPattern(char * SetPattern, const char * TheString)
			{
				/**
				 * true to results.
				 */
				bool						Results = false;

				if (SetPattern != NULL && TheString != NULL) {
					if (*SetPattern == '%') {

					}
				}

				return(Results);
			}

			EXPORT_OUT bool
				IsPattern(Terminal * SetPattern, const char * TheString)
			{
				bool						Results = false;

				return(Results);
			}

			EXPORT_OUT char	*
				GetName(char ** ParseAt)
			{
				char				*	Results = NULL;
				
				if (ParseAt != NULL && *ParseAt != NULL) {
					if (isalpha(**ParseAt)) {
					
						size_t			Len = strspn(*ParseAt, ValidInName);

						if (Len > 0) {
							Results = new char[Len + 1];

							strncpy(Results, *ParseAt, Len);
							Results[Len] = '\0';
							MoveForward(ParseAt, Len);
						}
					}
				}
				
				return(Results);
			}

			EXPORT_OUT char	*
				LookName(char ** ParseAt)
			{
				char				*	Results = NULL;
				const char			*	Copy = NULL;

				if (ParseAt != NULL && *ParseAt != NULL) {
					Copy = *ParseAt;

					if (isalpha(*Copy)) {
					
						size_t			Len = strspn(Copy, ValidInName);

						if (Len > 0) {
							Results = new char[Len + 1];

							strncpy(Results, Copy, Len);
							Results[Len] = '\0';

							Copy += Len;
						}
					}
				}

				return(Results);
			}


			EXPORT_OUT char	*
				GetDigits(char ** ParseAt)
			{
				char				*	Results = NULL;
				static const char	*	Valid = "0123456789";

				if (ParseAt != NULL && *ParseAt != NULL) {
					
					size_t			Len = strspn(*ParseAt, Valid);

					if (Len > 0) {
						///< An enum constant representing the results option
						Results = new char[Len + 1];

						strncpy(Results, *ParseAt, Len);
						Results[Len] = '\0';
						MoveForward(ParseAt, Len);
					}
				}
				
				return(Results);
			}

			EXPORT_OUT char
				MoveForward(char ** ParseAt, size_t MoveAheadCount)
			{
				char						Results = 0;

				if (ParseAt != NULL && *ParseAt != NULL) {
					while (MoveAheadCount > 0) {
						if (**ParseAt == '\n') {
							LineNumber++;
							CharacterPosition = 0;
						}
						(*ParseAt)++;
						CharacterPosition++;
						MoveAheadCount--;
						if (**ParseAt == '\0') {
							break;
						}
					}
					Results = **ParseAt;
				}

				return(Results);
			}
		
		}
	}
}
