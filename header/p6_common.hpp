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

	///Real number
	typedef double real;

	///Unsigned integer number
	typedef size_t uint;

	///Returns square of real number
	real sqr(real x) noexcept;

	///Returns pi number
	real pi() noexcept;

	///Coordinate vector
	struct Coord
	{
		real x;												///<X coordinate
		real y;												///<Y coordinate
		Coord()								noexcept;		///<Creates zero coordinates
		Coord(real x, real y)				noexcept;		///<Creates coordinates with given components
		Coord operator+(Coord coord)		const noexcept;	///<Returns sum of two coordinates
		Coord operator-(Coord coord)		const noexcept;	///<Returns difference between two coordinates
		Coord operator*(real coef)			const noexcept;	///<Returns coordinates multiplied with scalar value
		Coord operator/(real coef)			const noexcept;	///<Returns coordinates divided with scalar value
		real distance(Coord coord)			const noexcept;	///<Returns distance to coordinates
		real distance(const Coord coord[2])	const noexcept;	///<Returns distance to line or NaN
		real norm()							const noexcept;	///<Returns norm of coordinates (considered as vector)
		real angle()						const noexcept;	///<Returns angle of coordinates (considered as vector)
		Coord rotate(real angle)			const noexcept;	///<Returns coordinates rotated by some angle (considered as vector)
	};

	///String used in P6
	typedef std::string String;
	
	///Converts string ro real
	real string_to_real(const String s) noexcept;

	///Converts real to string
	String real_to_string(real r) noexcept;

}

#endif