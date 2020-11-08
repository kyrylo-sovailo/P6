#ifndef P6_MAIN_PANEL
#define P6_MAIN_PANEL

#include <wx/wx.h>
#include <set>

namespace p6
{
	class Frame;

	class MainPanel
	{
	private:
		Frame *_frame;
		wxPoint _real_to_pixel(real x, real y, wxPoint offset) const;

	public:
		enum class Item
		{
			no,
			node,
			stick,
			force
		};

		wxPanel *panel;
		real pixels_in_meter = 30.0;
		real meters_in_newton = 1.0;
		real center_x = 0.0;
		real center_y = 0.0;
		std::set<size_t> selected_nodes;
		std::set<size_t> selected_sticks;
		std::set<size_t> selected_forces;

		MainPanel(Frame *frame);
		wxSize size()										const;
		wxPoint offset()									const;
		void render(wxDC *dc, wxPoint offset)				const;
		Item get_item(real x, real y, size_t *index)		const;
		void pixel_to_real(wxPoint point, real *x, real *y)	const;
		void select_area(real ax, real ay, real bx, real by);
		void refresh();
	};
};

#endif