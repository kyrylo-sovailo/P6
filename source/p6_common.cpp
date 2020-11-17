/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_common.hpp"

#include <cmath>
#include <sstream>

p6::real p6::string_to_real(const String s) noexcept
{
	std::stringstream ss;
	real r;
	ss << s;
	if (ss >> r) return r;
	else return nan("");
}

p6::String p6::real_to_string(real r) noexcept
{
	std::stringstream ss;
	String s;
	ss << r;
	ss >> s;
	return s;
}

p6::real p6::sqr(real x) noexcept
{
	return x * x;
}

p6::real p6::pi() noexcept
{
	return 3.14159265358979323846;
}

p6::Coord::Coord() noexcept : x(0.0), y(0.0)
{
}

p6::Coord::Coord(real x, real y) noexcept : x(x), y(y)
{
}

p6::Coord p6::Coord::operator+(Coord coord) const noexcept
{
	return Coord(x + coord.x, y + coord.y);
}

p6::Coord p6::Coord::operator-(Coord coord) const noexcept
{
	return Coord(x - coord.x, y - coord.y);
}

p6::Coord p6::Coord::operator*(real coef) const noexcept
{
	return Coord(x * coef, y * coef);
}

p6::Coord p6::Coord::operator/(real coef) const noexcept
{
	return Coord(x / coef, y / coef);
}

p6::real p6::Coord::distance(Coord coord) const noexcept
{
	return sqrt(sqr(x - coord.x) + sqr(y - coord.y));
}

p6::real p6::Coord::distance(const Coord coord[2]) const noexcept
{
	real k = (
		(coord[1].x - x) * (coord[1].x - coord[0].x) +
		(coord[1].y - y) * (coord[1].y - coord[0].y)
		) / (sqr(coord[1].x - coord[0].x) + sqr(coord[1].y - coord[0].y));

	if (k < 0 || k > 1) return nan("");
	else return sqrt(
		sqr(k * coord[0].x + (1.0 - k) * coord[1].x - x) +
		sqr(k * coord[0].y + (1.0 - k) * coord[1].y - y));
}

p6::real p6::Coord::norm() const noexcept
{
	return sqrt(sqr(x) + sqr(y));
}

p6::real p6::Coord::angle() const noexcept
{
	if (x == 0.0 && y == 0.0) return 0.0;
	else if (abs(x) > abs(y))
	{
		if (x > 0) return atan(y / x);
		return pi() + atan(y / x);
	}
	else
	{
		if (y > 0) return 2.0 * pi() - atan(x / y);
		return 1.5 * pi() - atan(x / y);
	}
}

p6::Coord p6::Coord::rotate(real angle) const noexcept
{
	return Coord(cos(angle) * x - sin(angle) * y, sin(angle) * x + cos(angle) * y);
}