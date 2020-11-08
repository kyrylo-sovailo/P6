/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_UTILS
#define P6_UTILS

#include "p6_common.hpp"
#include "p6_construction.hpp"
#include <wx/wx.h>
#include <wx/file.h>
#include <string>

namespace p6
{
	class Utils
	{
	public:
		static real string_to_real(const String s);
		static String real_to_string(real r);
		static wxBitmap load_png(const String path);
		static real distance_to_line(real px, real py, const real x[2], const real y[2]);
	};
}

#endif
