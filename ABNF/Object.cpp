/**
 */

#include "Object.hpp"
#include "ABNF.hpp"

#include <ctype.h>

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{

			EXPORT_OUT
				Object::Object()
			{
				this->_DataType = IsAnd;
				this->Min = 1;
				this->Max = 1;
				this->IsResolved = false;
				this->IsBeingResolved = false;

				return;
			}

			char	*
				Object::_PrintRepetition() const
			{
				char					*	Results = NULL;

				if (this->Min != 1 || this->Max != 1) {
					char				*	big = new char[100];

					if (this->Min != 0 && this->Max != 0) {
						if (this->Min != this->Max) {
							sprintf(big, "%u*%u", this->Min, this->Max);
						} else {
							sprintf(big, "%u", this->Min);
						}
					
					} else if (this->Min == 0 && this->Max != 0) {
						sprintf(big, "*%u", this->Max);

					} else if (this->Min != 0 && this->Max == 0) {
						sprintf(big, "%u*", this->Min);
					
					} else if (this->Min == 0 && this->Max == 0) {
						sprintf(big, "*");
					}

					Results = strdup(big);
					delete big;
				}

				return(Results);
			}

			// Variable Repetition rule?
			// <a>*<b>element	A to B instances
			// <a>element		A instances
			// *element			Zero or more instances.
			//
			EXPORT_OUT bool
				Object::ParseRepetition(char ** ParseAt)
			{
				size_t						TheMin = 0;
				size_t						TheMax = 0;

				bool						Repetition = ParseRepetition(ParseAt, TheMin, TheMax);

				this->Min = TheMin;
				this->Max = TheMax;

				return(Repetition);
			}

			EXPORT_OUT bool
				Object::ParseRepetition(char ** ParseAt, size_t & MinRep, size_t & MaxRep)
			{
				bool						Results = false;

				if (isdigit(**ParseAt) || **ParseAt == '*') {
					Results = true;

					if (isdigit(**ParseAt)) {
						char			*	TheDigits = GetDigits(ParseAt);

						/* default to '<a>element' */
						MinRep = atoi(TheDigits);
						MaxRep = MinRep;

						// GetDigits has skipped any digits.
						// 
						if (**ParseAt == '*') {
							MoveForward(ParseAt, 1);
							if (isdigit(**ParseAt)) {
								/* is '<a>*<b>element' */
								TheDigits = GetDigits(ParseAt);
								MaxRep = atoi(TheDigits);
							} else {
								/* is '<a>*element */
								MaxRep = 0;
							}
						}
					} else {
						/* is '*element' */
						MinRep = 0;
						MaxRep = 0;
						MoveForward(ParseAt, 1);
					}				
				}

				return(Results);
			}
		}
	}
}
