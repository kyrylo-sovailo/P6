#ifndef P6_COMMON
#define P6_COMMON

#include <string>

namespace p6
{
	///String used in P6
	typedef std::string String;

	///Real number used in P6 calculations
	typedef double real;

	///Square of real number
	real sqr(real x);

	///Square of signed integer number
	int sqr(int x);
};

#endif