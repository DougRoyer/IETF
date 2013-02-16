/**
 */

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

#include "RuleName.hpp"
#include "ABNF.hpp"
#include <string.h>
#include <stdlib.h>

namespace SoftwareAndServices
{
	namespace Library
	{
		namespace ABNF
		{

			EXPORT_OUT const char		*	RuleNameStart = "abcdefghijklmnopqrstuvwxyz"
															"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

			EXPORT_OUT const wchar_t	*	WRuleNameStart = L"abcdefghijklmnopqrstuvwxyz"
																L"ABCDEFGHIJKLMNOPQRSTUVWXYZ";


			EXPORT_OUT const char		*	RuleNameCharacter = "abcdefghijklmnopqrstuvwxyz"
																"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
																"0123456789-";

			EXPORT_OUT const wchar_t	*	WRuleNameCharacter = L"abcdefghijklmnopqrstuvwxyz"
																L"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
																L"0123456789-";


			const char					*	WhiteSpace = " \n\r\f";

			EXPORT_OUT
				RuleName::RuleName()
			{
				this->_RuleName = NULL;

				return;
			}

			EXPORT_OUT
				RuleName::~RuleName()
			{
				if (this->_RuleName != NULL) {
					delete this->_RuleName;
					this->_RuleName = NULL;
				}

				return;
			}

			EXPORT_OUT const char	*
				RuleName::Name() const
			{
				return(this->_RuleName);
			}

			EXPORT_OUT bool
				RuleName::Name(char * TheRuleName)
			{
				bool					Results = false;

				if (TheRuleName != NULL) {
					if (IsName(TheRuleName)) {
						this->_RuleName = strdup(TheRuleName);
						Results = true;
					}
				}

				return(Results);
			}

			EXPORT_OUT bool
				RuleName::Name(wchar_t * TheRuleName)
			{
				bool						Results = false;

				if (TheRuleName != NULL) {
					size_t					WLen = wcslen(TheRuleName);
					char				*	TheName = new char[WLen + 1];
					size_t					Converted = wcstombs(TheName, TheRuleName, WLen);

					if (Converted == WLen) {
						Results = Name(TheName);
						delete TheName;
					}
				}

				return(Results);
			}

			EXPORT_OUT bool
				RuleName::IsName(char * TheRuleName, size_t * NameLen) const
			{
				bool				Results = false;

				if (TheRuleName != NULL) {
					size_t				TheNameLen = strlen(TheRuleName);
					size_t				Len = strspn(TheRuleName, RuleNameStart);

					if (Len > 0) {
						if (TheNameLen == Len) {
							if (NameLen != NULL) {
								*NameLen = Len;
							}
							Results = true;

						} else {
							Len = strspn(TheRuleName, RuleNameCharacter);

							if (Len == TheNameLen) {
								if (NameLen != NULL) {
									*NameLen = Len;
								}
								Results = true;

							} else {
								// If the initial segment up to the first WhiteSpace is a valid
								// rule name, use that.
								//
								if (TheRuleName[Len] == ' '
									|| TheRuleName[Len] == '\r'
									|| TheRuleName[Len] == '\n'
									|| TheRuleName[Len] == '\f')
								{
									if (NameLen != NULL) {
										*NameLen = Len;
									}
									Results = true;
								}
							}
						}
					}
				}

				return(Results);
			}

			EXPORT_OUT bool
				RuleName::IsName(wchar_t * TheRuleName, size_t * WcharCount) const
			{
				bool				Results = false;

				if (TheRuleName != NULL) {
					size_t				TheNameLen = wcslen(TheRuleName);
					size_t				Len = wcsspn(TheRuleName, WRuleNameStart);

					if (Len > 0) {
						if (TheNameLen == Len) {
							if (WcharCount != NULL) {
								*WcharCount = Len;
							}
							Results = true;

						} else {
							Len = wcsspn(TheRuleName, WRuleNameCharacter);

							if (Len == TheNameLen) {
								if (WcharCount != NULL) {
									*WcharCount = Len;
								}
								Results = true;

							} else {
								// If the initial segment up to the first WhiteSpace is a valid
								// rule name, use that.
								//
								if (TheRuleName[Len] == ' '
									|| TheRuleName[Len] == '\r'
									|| TheRuleName[Len] == '\n'
									|| TheRuleName[Len] == '\f')
								{
									if (WcharCount != NULL) {
										*WcharCount = Len;
									}
									Results = true;
								}
							}
						}
					}
				}

				return(Results);
			}

			EXPORT_OUT bool
				RuleName::Parse(char ** ParseAt)
			{
				bool					Results = false;

				if (this->_RuleName != NULL) {
					delete this->_RuleName;
					this->_RuleName = NULL;
				}

				if (ParseAt != NULL && *ParseAt != NULL) {
					size_t				Len = IsName(*ParseAt);

					if (Len > 0) {
						this->_RuleName = new char[Len + 1];
						strncpy(this->_RuleName, *ParseAt, Len);
						this->_RuleName[Len] = '\0';
						MoveForward(ParseAt, Len);
						Results = true;
					}
				}

				return(Results);
			}

			EXPORT_OUT char	*
				RuleName::Print() const
			{
				char			*	Results = NULL;

				if (this->_RuleName != NULL) {
					Results = strdup(this->_RuleName);
				}

				return(Results);
			}

		}
	}
}
