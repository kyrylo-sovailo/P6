/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_COMMON
#define P6_COMMON

#include <string>

namespace p6
{
	///String used in P6
	typedef std::string String;

	///Real number used in P6
	typedef double real;

	///uinteger number used in P6
	typedef size_t uint;

	///Square of real number
	real sqr(real x);
}

#endif