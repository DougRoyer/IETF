/**
 */

#include "ABNF.hpp"
#include "Rule.hpp"
#include "Comment.hpp"
#include <stdio.h>

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{

			EXPORT_OUT
				Comment::Comment()
			{
				this->_DataType = IsComment;
				this->_Comment = NULL;

				return;
			}

			EXPORT_OUT
				Comment::~Comment()
			{
				if (this->_Comment != NULL) {
					delete this->_Comment;
					this->_Comment = NULL;
				}

				return;
			}

			EXPORT_OUT bool
				Comment::Parse(char ** ParseAt)
			{
				bool						Results = false;

				if (ParseAt != NULL && *ParseAt != NULL) {
					if (**ParseAt == ';') {

						MoveForward(ParseAt, 1);	// Skip past ';'

						char			*	eol = strchr(*ParseAt, '\r');

						if (eol != NULL) {
							size_t			Len = eol - *ParseAt;
							
							this->_Comment = new char[Len + 1];
							strncpy(this->_Comment, *ParseAt, Len);
							this->_Comment[Len] = '\0';
							MoveForward(ParseAt, Len);
							Results = true;

						}
					}
				}
				return(Results);				
			}
			
			EXPORT_OUT char	*
				Comment::Print(bool WithRepetitions) const
			{
				char				*	Results = NULL;

				if (this->_Comment != NULL) {
					size_t				Len = strlen(this->_Comment);

					Results = strdup(this->_Comment);
				}

				return(Results);
			}
		

			EXPORT_OUT bool
				Comment::IsTerminal() const
			{
				return(true);
			}

		}
	}
}
