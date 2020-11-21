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

#include "p6_common.hpp"
#include <wx/wx.h>
#include <set>

namespace p6
{
	class Frame;

	///Main panel that displays construction graphically
	class MainPanel
	{
	private:
		Frame *_frame;					///<Application's window
		wxPanel *_panel;				///<wxWidet's panel
		wxPoint _area_select_begin;		///<Begin point of area selecting
		wxPoint _area_select_current;	///<Current point of area selecting (needs to be saved because rendering happens in another function)
		bool _area_select_draw;			///<Indicator if area selection needs to be drawn
		bool _grid_draw;				///<Indicator if grid needs to be drawn
		Coord _old_center;				///<Coordinates of old view center when being dragged
		wxPoint _drag_begin;			///<Begin point of dragging

		wxPoint _real_to_pixel(Coord coord, wxPoint offset)	const noexcept;	///<Transforms meter coordinates to pixel coordinates

	public:
		///Item that could be displayed on main panel
		struct Item
		{
			enum class Type
			{
				no,
				node,
				stick,
				force,
				anchor
			};

			Type type;		///Type of item
			uint index;		///Index of item, valid for node, stick and force
			bool selected;	///Indicator if item is selected
		};

		Coord center;												///<Coordinates of view center
		real pixels_in_meter = 30.0;								///<Pixels per meter rate
		real meters_in_newton = 1.0;								///<Meters on newton rate (to display forces)
		std::set<uint> selected_nodes;								///<Selected nodes
		std::set<uint> selected_sticks;								///<Selected sticks
		std::set<uint> selected_forces;								///<Selected forces
		
		MainPanel(Frame *frame)					noexcept;			///<Creates main panel
		wxPanel *panel()						noexcept;			///<Returns wxWidet's panel
		wxSize size()							const noexcept;		///<Returns Panel's size
		void render(wxDC *dc, wxPoint offset)	const noexcept;		///<Renders into given drawing context with given offset
		Item get_item(wxPoint point)			const noexcept;		///<Returns item located in given coordinates
		Coord pixel_to_real(wxPoint point)		const noexcept;		///<Transforms pixel coordinates to meter coordinates
		void area_select_begin(wxPoint point)	noexcept;			///<Begins area selecting
		void area_select_continue(wxPoint point)noexcept;			///<Continues area selecting
		void area_select_end(wxPoint point)		noexcept;			///<Ends area selecting, selects items
		void drag_begin(wxPoint point)			noexcept;			///<Begins view dragging
		void drag_continue(wxPoint point)		noexcept;			///<Continues view dragging
		void grid_draw(bool grid)				noexcept;			///<Sets if grid needs to be drawn
		void need_refresh()						noexcept;			///<Indicats that image needs to be redrawn
	};
}

#endif