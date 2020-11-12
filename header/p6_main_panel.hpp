/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

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
		wxPanel *_panel;
		
		wxPoint _real_to_pixel(Coord coord, wxPoint offset)	const;

	public:
		///Structure representing item type and it's inddex
		struct Item
		{
			enum class Type
			{
				no,
				node,
				stick,
				force
			};

			Type type;
			uint index;
		};

		real pixels_in_meter = 30.0;
		real meters_in_newton = 1.0;
		Coord center;
		std::set<uint> selected_nodes;
		std::set<uint> selected_sticks;
		std::set<uint> selected_forces;
		wxPoint selected_area_points[2];
		bool selected_area_draw;

		MainPanel(Frame *frame);
		wxPanel *panel();
		wxSize size()							const;
		void render(wxDC *dc, wxPoint offset)	const;
		Item get_item(wxPoint a)				const;
		Coord pixel_to_real(wxPoint point)		const;
		void select_items();
		void need_refresh();
	};
}

#endif