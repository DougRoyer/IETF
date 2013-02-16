// This is the main DLL file.

#include <ctype.h>
#include <string.h>
#include "Parser.hpp"

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{
		
			EXPORT_OUT
				Parser::Parser()
			{
				this->_Topmost = NULL;
				this->_ParseAt = NULL;
				this->_OurParseAt = false;

				return;
			}

			EXPORT_OUT
				Parser::~Parser()
			{
				
				if (this->_OurParseAt) {
					delete this->_ParseAt;
					this->_OurParseAt = false;
				}
				this->_ParseAt = NULL;
				this->_Topmost = NULL;
				
				return;
			}

			EXPORT_OUT void
				Parser::Text(char * ParseAt)
			{
				this->_ParseAt = ParseAt;
				this->_OurParseAt = false;

				return;
			}

			EXPORT_OUT void
				Parser::Text(wchar_t * WParseAt)
			{
				if (WParseAt != NULL) {
					size_t				WLen = wcslen(WParseAt);

					if (WLen > 0) {
						char		*	ParseAt = new char[WLen + 1];

						size_t			Converted = wcstombs(ParseAt, WParseAt, WLen);

						if (Converted == WLen) {
							this->_ParseAt = ParseAt;
							this->_OurParseAt = true;
						} else {
							delete ParseAt;
							this->_ParseAt = NULL;
						}
					}
				}

				return;
			}

			EXPORT_OUT bool
				Parser::Parse()
			{
				bool					Results = false;

				bool					Worked = false;

				Rule				*	RuleToParse = NULL;

				if (this->_ParseAt != NULL) {

					this->_Topmost = new Rule(*this);
					RuleToParse = this->_Topmost;

					do {
						while (SkipCRLF(&this->_ParseAt) != 0);
						Worked = RuleToParse->Parse(&this->_ParseAt);
						if (Worked) {
							Results = true;
						} else {
							Results = false;
						}

						if (*this->_ParseAt != '\0') {
							SkipWs(&this->_ParseAt);
							SkipCRLF(&this->_ParseAt);
							SkipWs(&this->_ParseAt);

							char		*	NextName = LookName(&this->_ParseAt);

							if (NextName != NULL) {
								RuleToParse = this->Add(NextName);
							}
						}
					} while (*this->_ParseAt != '\0');
				}

				if (Results) {
					Results = false;

					// Now go through all rules and make sure they all resolve to only terminals.
					//
					std::vector<Rule*>::const_iterator	RuleIt;
					Rule							*	aRule = NULL;

					for (RuleIt = this->_Rules.begin(); RuleIt != this->_Rules.end(); RuleIt++) {
						aRule = *RuleIt;
						if (aRule != NULL) {
							if (!_Resolved(aRule)) {
								break;
							}
						}
					}
				
					if (RuleIt == this->_Rules.end()) {
						Results = true;
					}
				}

				return(Results);
			}

			bool
				Parser::_Resolved(Rule * RuleToCheck)
			{
				bool									Results = false;

				std::vector<Rule*>::const_iterator		rIt;
				Rule								*	aRule = NULL;

				const std::vector<Object*>			*	ElementList = NULL;
				std::vector<Object*>::const_iterator	eIt;
				Object								*	AnObject = NULL;

				RuleToCheck->IsBeingResolved = true;
				for (rIt = this->_Rules.begin(); rIt != this->_Rules.end(); rIt++) {
					aRule = *rIt;
					if (aRule != NULL) {
						if (aRule != RuleToCheck) {
							continue;
						}
						if (aRule->IsResolved) {
							Results = true;
							break;
						}
						if (aRule->IsTerminal()) {
							aRule->IsResolved = true;
							Results = true;
							break;
						}

						// Check its Elements
						//
						ElementList = aRule->Elements();

						if (ElementList->size() > 0) {
							for (eIt = ElementList->begin(); eIt != ElementList->end(); eIt++) {
								AnObject = *eIt;
								if (AnObject != NULL) {
									if (AnObject->IsResolved || AnObject->IsBeingResolved) {
										continue;
									}
									if (AnObject->IsTerminal()) {
										AnObject->IsResolved = true;
										continue;
									}
									if (this->_Resolved((Rule*)AnObject)) {
										AnObject->IsResolved = true;
										continue;
									}
									break;
								}
							}
						
							if (eIt == ElementList->end()) {
								aRule->IsResolved = true;
								Results = true;
								break;
							}
						}
					}
				}
				
				return(Results);
			}

			EXPORT_OUT Rule	*
				Parser::Top() const
			{
				return(this->_Topmost);
			}

			EXPORT_OUT char	*
				Parser::Print() const
			{
				char							*	Results = NULL;

				std::vector<Rule*>::const_iterator	rIt;
				Rule							*	aRule = NULL;
				std::vector<char*>					CharResults;
				size_t								Total = 0;
				char							*	aValue = NULL;

				for (rIt = this->_Rules.begin(); rIt != this->_Rules.end(); rIt++) {
					aRule = *rIt;
					if (aRule != NULL) {
						aValue = aRule->Print(false);
						if (aValue != NULL) {
							Total += strlen(aValue) + 2;
							CharResults.push_back(aValue);
						}
					}
				}

				std::vector<char*>::iterator		cIt;
				
				Results = new char[Total + (CharResults.size() *  20)];
				Results[0] = '\0';

				for (cIt = CharResults.begin(); cIt != CharResults.end(); cIt++) {
					aValue = *cIt;
					strcat(Results, aValue);
					delete aValue;
				}
				
				return(Results);
			}

			EXPORT_OUT Rule *
				Parser::Add(Rule * TheRule)
			{
				
				std::vector<Rule*>::const_iterator	it;
				Rule			*	aRule = NULL;

				for (it = this->_Rules.begin(); it != this->_Rules.end(); it++) {
					aRule = *it;
					if (aRule == TheRule) {
						break;
					}
				}

				if (it == this->_Rules.end()) {
					this->_Rules.push_back(TheRule);
				}

				return(TheRule);
			}

			EXPORT_OUT Rule	*
				Parser::Add(char * TheRuleName)
			{
				Rule			*	Results = NULL;

				std::vector<Rule*>::const_iterator	it;
				Rule			*	aRule = NULL;

				for (it = this->_Rules.begin(); it != this->_Rules.end(); it++) {
					aRule = *it;
					if (aRule != NULL) {
						if (strcasecmp(TheRuleName, aRule->Name()) == 0) {
							Results = aRule;
							break;
						}
					}
				}

				if (Results == NULL) {
					Results = new Rule(*this);
					Results->Name(TheRuleName);
					this->_Rules.push_back(Results);
				}

				return(Results);
			}

			EXPORT_OUT const std::vector<Rule*>	*
				Parser::Rules() const
			{
				return(&this->_Rules);
			}

		}
	}
}
