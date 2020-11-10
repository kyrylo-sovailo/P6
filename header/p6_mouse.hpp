/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_MOUSE_BAR
#define P6_MOUSE_BAR

#include "p6_common.hpp"
#include "p6_main_panel.hpp"
#include <wx/wx.h>

namespace p6
{
	class Frame;

	struct Mouse
	{
	private:
		Frame *_frame;
		bool _pressed;
		MainPanel::Item _pressed_item;
		wxPoint _pressed_point;
		bool _moving;
		int _wheel;
		int _wheel_force;
		Coord _old_center;

		void _on_left_down(wxMouseEvent &e);
		void _on_left_up(wxMouseEvent &e);
		void _on_move(wxMouseEvent &e);
		void _on_wheel(wxMouseEvent &e);

	public:
		Mouse(Frame *frame);
	};
}

#endif