/**
 * @file Generate.cpp
 * This file implements the interface to generate code.
 *
 * @author Douglas Mark Royer <DouglasRoyer@gmail.com>
 */
#include "Generate.hpp"

#include "C++/CPlusPlus.hpp"

int
	Generate()
{
	int							Results = 0;

	switch (SelectedLanguage) {

	case CPlusPlus_l:
		Results = GenerateCPlusPlus();
		break;

	case Ruby_l:
		Results = 1;	// Your burnt - not ready.
		break;

	default:
		Results = 1;	// Not supported.
		break;
	}

	return(Results);
}
