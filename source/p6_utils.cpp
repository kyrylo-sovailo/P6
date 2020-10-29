#include "../header/p6_utils.h"
#include <sstream>

p6::real p6::Utils::string_to_real(const String s)
{
	std::stringstream ss;
	real r;
	ss << s;
	if (ss >> r) return r;
	else return nan("");
};

std::string p6::Utils::real_to_string(real r)
{
	std::stringstream ss;
	std::string s;
	ss << r;
	ss >> s;
	return s;
};

wxBitmap p6::Utils::load_png(const String path)
{
	if (!wxFileExists(path)) return wxNullBitmap;
	wxFile file(path, wxFile::OpenMode::read);
	if (!file.IsOpened()) return wxNullBitmap;
	file.Seek(0, wxSeekMode::wxFromEnd);
	std::vector<char> memory(file.Tell());
	file.Seek(0, wxSeekMode::wxFromStart);
	file.Read(&memory[0], memory.size());
	wxBitmap bitmap;
	{
		wxLogNull lognull;
		bitmap = wxBitmap::NewFromPNGData(memory.data(), memory.size());
	}
	return bitmap;
};

p6::real p6::Utils::distance_to_line(real px, real py, const real x[2], const real y[2])
{
	real k = ((x[1] - px) * (x[1] - x[0]) + (y[1] - py) * (y[1] - y[0])) /
		(sqr(x[1] - x[0]) + sqr(y[1] - y[0]));
	
	if (k < 0 || k > 1) return nan("");
	else return sqrt(
		sqr(k * x[0] + (1.0 - k) * x[1] - px) +
		sqr(k * y[0] + (1.0 - k) * y[1] - py));
};