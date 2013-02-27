/**
 */

#include "ABNF.hpp"
#include "Parser.hpp"
#include "Rule.hpp"
#include "Terminal.hpp"
#include "Comment.hpp"

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{
			static const char				*	UnResolved = " ; ERROR: This Rule is UN-Resolved.";

			EXPORT_OUT
				Rule::Rule(Parser & Parent)
				: _Parent(&Parent)
			{
				this->IsResolved = false;

				return;
			}

			EXPORT_OUT
				Rule::~Rule()
			{
				this->IsResolved = false;
				this->_Parent = NULL;

				std::vector<Object*>::iterator			it;
				Object								*	AnObject = NULL;

				for (it = this->_Elements.begin(); it != this->_Elements.end(); it++) {
					AnObject = *it;
					if (AnObject != NULL) {
						delete AnObject;
					}
				}

				return;
			}

			EXPORT_OUT bool
				Rule::Parse(char ** ParseAt)
			{
				bool						Results = false;

				if (**ParseAt != '\0') {
				
					SkipCRLF(ParseAt);

					if (**ParseAt == ';') {
						Comment			*	aNewComment = new Comment();

						if (aNewComment->Parse(ParseAt)) {
							SkipWs(ParseAt);
							SkipCRLF(ParseAt);
							this->_Elements.push_back(aNewComment);
						}
					}

					// Get a name.
					//
					SkipWs(ParseAt);
					char				*	AToken = GetName(ParseAt);

					if (AToken != NULL) {
						Terminal		*	ATerminal = NULL;
						Rule			*	ARule = NULL;
						bool				HasRepetition = false;
						size_t				MinReps = 1;
						size_t				MaxReps = 1;

						SkipWs(ParseAt);
				
						// Should be an equal sign '=' or another Element value, or CRLF.
						//
						if (**ParseAt == '=') {
							if (*((*ParseAt) + 1) == '/') {

								ATerminal = new Terminal();
								if (ATerminal->Parse(ParseAt)) {
									SkipWs(ParseAt);
									this->_Elements.push_back(ATerminal);
								}

							} else {
								MoveForward(ParseAt, 1);	// Skip '='
							}

							this->Name(AToken);
							
							while (**ParseAt != '\0') {

								SkipWs(ParseAt);

								if (**ParseAt == ';') {	// Comment
									Comment			*	NewComment = new Comment();

									if (NewComment->Parse(ParseAt)) {
										SkipWs(ParseAt);
										SkipCRLF(ParseAt);
										this->_Elements.push_back(NewComment);
									}
									continue;
								}

								MinReps = 1;
								MaxReps = 1;
								HasRepetition = this->ParseRepetition(ParseAt, MinReps, MaxReps);

								if (**ParseAt == '%'	// %d, %x, or %b
									|| **ParseAt == '"'	// String
									|| **ParseAt == '('	// Group Start
									|| **ParseAt == ')'	// Group End
									|| **ParseAt == '<'	// Prose Start
									|| **ParseAt == '>'	// Prose End
									|| **ParseAt == '['	// Option Start
									|| **ParseAt == ']'	// Option End
									|| **ParseAt == '/'	// Or
									|| **ParseAt == ';' // Comment
									)
								{
									ATerminal = new Terminal();

									if (HasRepetition) {
										ATerminal->Min = MinReps;
										ATerminal->Max = MaxReps;
									}
									if (ATerminal->Parse(ParseAt)) {
										SkipWs(ParseAt);
										this->_Elements.push_back(ATerminal);;
									}
									continue;
								}

								// Must be a Rule
								//
								SkipWs(ParseAt);
								//SkipCRLF(ParseAt);
								AToken = GetName(ParseAt);

								if (AToken != NULL) {
									Rule	*	TheNewRule = new Rule(*this->_Parent);

									TheNewRule->Name(AToken);
									TheNewRule->IsElement = true;
									if (HasRepetition) {
										TheNewRule->Min = MinReps;
										TheNewRule->Max = MaxReps;
									}
									this->_Elements.push_back(TheNewRule);
									SkipWs(ParseAt);
								}

								if (**ParseAt == '\r') {
									MoveForward(ParseAt, 1);
									if (**ParseAt == '\n') {
										MoveForward(ParseAt, 1);
										SkipCRLF(ParseAt);
										if (**ParseAt == '\t') {
											do {
												MoveForward(ParseAt, 1);
											} while (**ParseAt == '\t');
											continue;				// Wrapped line.
										}
										Results = true;
										break;
									}
								}
							}
						}
					}
				}
				return(Results);				
			}
			
			EXPORT_OUT char	*
				Rule::Print(bool WithRepetitions) const
			{
				char					*	Results = NULL;

				const char				*	TheName = Name();

				if (TheName != NULL) {
					// Collect a list of values to return.
					//
					std::vector<char *>		Values;
					size_t					Total = strlen(TheName) + 1;

					Values.push_back(strdup(TheName));
				
					std::vector<Object*>::const_iterator	it;

					Object				*	AnObject = NULL;
					char				*	AValue = NULL;
					const char			*	AName = NULL;

					if (this->_Elements.size() > 0) {
						for (it = this->_Elements.begin(); it != this->_Elements.end(); it++) {
							AnObject = *it;
							if (AnObject != NULL) {
								if (AnObject->IsTerminal()) {
									AValue = AnObject->Print(true);
								} else {
									char	*	Reps = ((Rule*)AnObject)->_PrintRepetition();
									AName = ((Rule*)AnObject)->Name();

									if (AName != NULL) {
										if (Reps != NULL) {
											size_t		RuleLen = strlen(Reps) + strlen(AName);
											char	*	RuleTmp = new char[RuleLen + 1];

											strcpy(RuleTmp, Reps);
											strcat(RuleTmp, AName);
											delete Reps;
											AValue = RuleTmp;
										} else {
											AValue = strdup(AName);
										}
									}
								}
								if (AValue != NULL) {
									Values.push_back(AValue);
									Total += strlen(AValue) + 1;
								}
							}
						}
					} else {
						AValue = strdup(UnResolved);
						Total += strlen(UnResolved);
						Values.push_back(AValue);
					}

					// Collect and return the results.
					//
					Results  = new char[Total + 20];
					Results[0] = '\0';

					std::vector<char*>::iterator		vIt;

					for (vIt = Values.begin(); vIt != Values.end(); vIt++) {
						AValue = *vIt;
						if (AValue != NULL) {
							strcat(Results, AValue);

							if (vIt == Values.begin()) {
								strcat(Results, " = ");
							} else {		
								strcat(Results, " ");
							}

							delete AValue;	
						}
					}
					strcat(Results, "\r\n");
				}

				return(Results);
			}

			EXPORT_OUT bool
				Rule::IsTerminal() const
			{
				return(false);
			}

			EXPORT_OUT const std::vector<Object*>	*
				Rule::Elements() const
			{
				return(&this->_Elements);
			}
		}
	}
}
