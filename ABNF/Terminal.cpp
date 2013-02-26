/**
 */

#include "ABNF.hpp"
#include "Rule.hpp"
#include "Terminal.hpp"
#include <stdio.h>

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{

			EXPORT_OUT
				Terminal::Terminal()
			{
				this->_DataType = IsOr;
				this->IsElement = true;		// Terminals are never rule definitions.
			
				return;
			}

			EXPORT_OUT
				Terminal::~Terminal()
			{
				
				std::vector<char*>::iterator	it;
				char						*	aValue = NULL;

				for (it = this->begin(); it != this->end(); it++) {
					if (aValue != NULL) {
						delete aValue;
					}
				}

				this->_DataType = IsOr;

				return;
			}

			EXPORT_OUT bool
				Terminal::Parse(char ** ParseAt)
			{
				bool				Results = false;

				bool				MightHaveRange = false;
				char			*	aValue = NULL;

				if (**ParseAt == '%') {
					MoveForward(ParseAt, 1);

					switch(**ParseAt) {

					case 'b':
						MoveForward(ParseAt, 1);
						aValue = ParseBinary(ParseAt);
						if (aValue != NULL) {

							this->_DataType = IsBinary;
							MightHaveRange = true;

							Results = true;
						}
						break;

					case 'd':
						MoveForward(ParseAt, 1);
						aValue = ParseDecimal(ParseAt);
						if (aValue != NULL) {

							this->_DataType = IsDecimal;
							MightHaveRange = true;

							Results = true;
						}
						break;

					case 'x':
						MoveForward(ParseAt, 1);
						aValue = ParseHexadecimal(ParseAt);
						if (aValue != NULL) {

							this->_DataType = IsHexadecimal;
							MightHaveRange = true;

							Results = true;
						}
						break;
					}

				} else if (**ParseAt == '(') {
					this->_DataType = GroupStart;
					MoveForward(ParseAt, 1);
					Results = true;
			
				} else if (**ParseAt == ')') {
					this->_DataType = GroupEnd;
					MoveForward(ParseAt, 1);
					Results = true;

				} else if (**ParseAt == '[') {
					this->_DataType = OptionStart;
					MoveForward(ParseAt, 1);
					Results = true;
			
				} else if (**ParseAt == ']') {
					this->_DataType = OptionEnd;
					MoveForward(ParseAt, 1);
					Results = true;

				} else if (**ParseAt == '<') {
					this->_DataType = ProseStart;
					MoveForward(ParseAt, 1);
					Results = true;
			
				} else if (**ParseAt == '>') {
					this->_DataType = ProseEnd;
					MoveForward(ParseAt, 1);
					Results = true;

				} else if (**ParseAt == '=' && *((*ParseAt) + 1) == '/') {
					this->_DataType = IsOr;
					MoveForward(ParseAt, 2);
					Results = true;

				} else if (**ParseAt == '/') {
					this->_DataType = IsOr;
					MoveForward(ParseAt, 1);
					Results = true;
				
				} else if (**ParseAt == '"') {

					this->_DataType = IsString;

					const char		*	StopAt = (*ParseAt) + 1;

					while (*StopAt != '"') {
						StopAt++;
					}
					
					size_t				Len = StopAt - *ParseAt;

					if (Len > 0) {
						aValue = new char[Len + 1];

						strncpy(aValue, (*ParseAt) + 1, Len - 1);
						aValue[Len - 1] = '\0';
						MoveForward(ParseAt, Len + 1);

						this->push_back(aValue);

						Results = true;
					}

				} else {
					// ERROR
					//
					throw InvalidTerminal_e;
				}

				if (Results) {
					if (aValue != NULL) {
						if (MightHaveRange) {
							// If it contains a '-' it is a range.
							//
							const char			*	Dash = strchr(aValue, '-');

							if (Dash != NULL) {
								
								this->IsRange = true;

								char			*	Old = aValue;
								size_t				Len = Dash - Old;
								char			*	Low = new char[Len + 1];

								strncpy(Low, Old, Len);
								Low[Len] = '\0';

								this->push_back(Low);
								this->push_back(strdup(Dash + 1));
								delete Old;

							} else {
								this->push_back(aValue);
							}

							// If it contains one or more '.' it is a set
							//
						}
					}
				}

				return(Results);				
			}
			
			EXPORT_OUT char	*
				Terminal::Print(bool WithRepetitions) const
			{
				char							*	Results = NULL;

				const char						*	Prefix = NULL;
				char							*	tmp = NULL;
				char							*	aValue = NULL;
				char							*	Repetitions = this->_PrintRepetition();
				bool								Done = false;
				size_t								Len = 1;

				switch (this->_DataType) {

					case IsOr:
						tmp = strdup("/");
						Done = true;
						break;

					case GroupStart:
						tmp = strdup("(");
						Done = true;
						break;

					case GroupEnd:
						tmp = strdup(")");
						Done = true;
						break;

					case OptionStart:
						tmp = strdup("[");
						Done = true;
						break;

					case OptionEnd:
						tmp = strdup("]");
						Done = true;
						break;

					case ProseStart:
						tmp = strdup("<");
						Done = true;
						break;

					case ProseEnd:
						tmp = strdup(">");
						Done = true;
						break;

				};

				if (Done) {
					if (Repetitions != NULL) {
						Len += strlen(Repetitions) + 1;

						aValue = new char[Len + 1];
						
						strcpy(aValue, Repetitions);
						strcat(aValue, tmp);
						delete tmp;
						tmp = aValue;
					}

				} else {
				
					std::vector<char*>::const_iterator	vIt;
					char							*	aValue = NULL;
				
					for (vIt = this->begin(); vIt != this->end(); vIt++) {
						aValue = *vIt;
						if (aValue != NULL) {
							Len += strlen(aValue) + 1;
						} else {
							Len += 4;
						}
					}

					tmp = new char[Len + (this->size() * 10)];
				
					static const char				*	BinaryPrefix = "%b";
					static const char				*	DecimalPrefix = "%d";
					static const char				*	HexadecimalPrefix = "%x";
					static const char				*	StringPrefix = "\"";
					static const char				*	RangePrefix = "";

					bool								DidDash = false;
				
					size_t								Offset = 0;
						
					tmp[0] = '\0';

					if (Repetitions != NULL) {
						strcat(tmp, Repetitions);
					}
					for (vIt = this->begin(); vIt != this->end(); vIt++, Offset++) {
						aValue = *vIt;
						if (aValue != NULL) {

							switch (this->_DataType) {

							case IsBinary:
								Prefix = BinaryPrefix;
								break;

							case IsDecimal:
								Prefix = DecimalPrefix;
								break;

							case IsHexadecimal:
								Prefix = HexadecimalPrefix;
								break;

							case IsString:
								Prefix = StringPrefix;
								break;

							}
						
							if (Offset == 0) {
								strcat(tmp, Prefix);
							}

							strcat(tmp, aValue);

							if (this->IsRange && !DidDash) {
								strcat(tmp, "-");
								DidDash = true;
							}

							if (this-> IsSet) {
								if (Offset < this->size()) {
									strcat(tmp, ".");
								}
							}

							if (this->_DataType == IsString) {
								strcat(tmp, StringPrefix);
							}
						}
					}
				}
				Results = strdup(tmp);
				
				return(Results);
			}
			
			EXPORT_OUT char	*
				Terminal::ParseBinary(char ** ParseAt)
			{
				char				*	Results =  GetUntilWs(ParseAt);

				return(Results);
			}

			EXPORT_OUT char	*
				Terminal::ParseDecimal(char ** ParseAt)
			{
				char				*	Results =  GetUntilWs(ParseAt);

				return(Results);
			}

			EXPORT_OUT char	*
				Terminal::ParseHexadecimal(char ** ParseAt)
			{
				char				*	Results =  GetUntilWs(ParseAt);

				return(Results);
			}

			EXPORT_OUT bool
				Terminal::IsTerminal() const
			{
				return(true);
			}

		}
	}
}
