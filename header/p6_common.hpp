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

	///Real number used in P6
	typedef double real;

	///uinteger number used in P6
	typedef size_t uint;

	///Square of real number
	real sqr(real x) noexcept;

	///Coordinate vector
	struct Coord
	{
		real x;
		real y;
		Coord()								noexcept;
		Coord(real x, real y)				noexcept;
		Coord operator+(Coord coord)		const noexcept;
		Coord operator-(Coord coord)		const noexcept;
		Coord operator*(real coef)			const noexcept;
		Coord operator/(real coef)			const noexcept;
		real distance(Coord coord)			const noexcept;
		real distance(const Coord coord[2])	const noexcept;
	};

	///String used in P6
	typedef std::string String;
	
	///Converts string ro real
	real string_to_real(const String s);

	///Converts real to string
	String real_to_string(real r);

}

#endif