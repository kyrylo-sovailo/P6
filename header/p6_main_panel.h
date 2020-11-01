#ifndef P6_MAIN_PANEL
#define P6_MAIN_PANEL

#include <wx/wx.h>
#include <set>

namespace p6
{
	class MainPanel
	{
	private:
		real _pixels_in_meter = 30;
		real _meters_in_newton = 1;
		real _x = 0.0;
		real _y = 0.0;

	public:
		size_t width() const;
		size_t height() const;
		void render(wxDC *dc);
	};
};

#endif