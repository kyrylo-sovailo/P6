#ifndef P6_UTILS
#define P6_UTILS

#include "p6_common.h"
#include "p6_construction.h"
#include <string>
#include <wx/wx.h>
#include <wx/file.h>

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
};
#endif
