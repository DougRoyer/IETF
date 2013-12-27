/**
 * @file Parser.cpp
 * This file implements the ABNF Parser object.
 *
 * @author Douglas Mark Royer <DouglasRoyer@gmail.com>
 */
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
				
				std::vector<const char*>::iterator	it;
				char							*	aValue = NULL;

				for (it = this->_ErrorMessages.begin(); it != this->_ErrorMessages.end(); it++) {
					aValue = (char*)*it;
					if (aValue != NULL) {
						delete aValue;
					}
				}

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

						if (this->_Rules.size() == 0) {
							this->_Rules.push_back(RuleToParse);
						}

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
					bool				SomethingFailed = false;

					Results = false;

					// Now go through all rules and make sure they all resolve to only terminals.
					//
					std::vector<Rule*>::const_iterator	RuleIt;
					Rule							*	aRule = NULL;

					for (RuleIt = this->_Rules.begin(); RuleIt != this->_Rules.end(); RuleIt++) {
						aRule = *RuleIt;
						if (aRule != NULL) {
							if (_Resolved(aRule)) {
								continue;
							}
							SomethingFailed = true;
						}
					}
				
					if (!SomethingFailed) {
						Results = true;
					}
				}

				return(Results);
			}

			void
				Parser::_MarkResolved(const char * TheName)
			{
				std::vector<Rule*>::const_iterator		rIt;
				Rule								*	aRule = NULL;

				for (rIt = this->_Rules.begin(); rIt != this->_Rules.end(); rIt++) {
					aRule = *rIt;
					if (aRule != NULL) {
						if (strcasecmp(TheName, aRule->Name()) == 0) {
							aRule->IsResolved = true;
						}
					}
				}

				return;
			}

			bool
				Parser::_Resolved(Rule * RuleToCheck)
			{
				bool										Results = false;

				if (RuleToCheck->IsResolved) {
					Results = true;
				
				} else {

					// Check all of its elements.
					//
					const std::vector<Object*>				*	Elements = RuleToCheck->Elements();
					size_t										ErrorCount = this->_ErrorMessages.size();

					if (Elements != NULL) {
						std::vector<Object*>::const_iterator	eIt;
						Object								*	anObject = NULL;
						Rule								*	aRule = NULL;
						Rule								*	FoundRule = NULL;
						char									tmp[1024];

						for (eIt = Elements->begin(); eIt != Elements->end(); eIt++) {
							anObject = *eIt;
							if (anObject != NULL) {
								if (anObject->IsTerminal()) {
									anObject->IsResolved = true;
									continue;
								}
								aRule = (Rule*) anObject;
								FoundRule = this->_FindRule(aRule->Name());
								if (FoundRule != NULL) {
									anObject->IsResolved = true;
									continue;
								}

								sprintf(tmp, "Rule %s, can not find definition for: %s", RuleToCheck->Name(), aRule->Name());
								this->_ErrorMessages.push_back(strdup(tmp));							
							}
						}
					}

					if (this->_ErrorMessages.size() > ErrorCount) {
						Results = false;

					} else {
						this->_MarkResolved(RuleToCheck->Name());
						Results = true;
					}
				}

				return(Results);
			}

			Rule	*
				Parser::_FindRule(const char * RuleName)
			{
				Rule							*	Results = NULL;

				if (RuleName != NULL) {
					std::vector<Rule*>::iterator	rIt;
					Rule						*	aRule = NULL;

					for (rIt = this->_Rules.begin(); rIt != this->_Rules.end(); rIt++) {
						aRule = *rIt;
						if (aRule != NULL) {
							if (strcasecmp(aRule->Name(), RuleName) == 0) {
								Results = aRule;
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
						if (aRule->IsElement) {
							continue;
						}
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

			EXPORT_OUT Rule	*
				Parser::Add(char * TheRuleName)
			{
				Rule			*	Results = new Rule(*this);

				Results->Name(TheRuleName);
				this->_Rules.push_back(Results);
				
				return(Results);
			}

			EXPORT_OUT const std::vector<Rule*>	*
				Parser::Rules() const
			{
				return(&this->_Rules);
			}

			EXPORT_OUT const std::vector<const char*>	*
				Parser::Errors() const
			{
				return(&this->_ErrorMessages);
			}

		}
	}
}
