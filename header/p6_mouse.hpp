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

	///Mouse events handler
	struct Mouse
	{
	private:
		Frame *_frame;							///<Application's window
		bool _pressed = false;					///<True if mouse button is pressed
		bool _moving = false;					///<True if mouse button was moved during press
		MainPanel::Item _pressed_item;			///<Item mouse was pressed on
		wxPoint _pressed_point;					///<Point mouse was pressed on
		int _wheel = 0;							///<State of mouse wheel used for scaling coordinates
		int _wheel_force = 0;					///<State of mouse wheel used for scaling forces

		void _on_left_down(wxMouseEvent &e);	///<Handles mouse press event
		void _on_left_up(wxMouseEvent &e);		///<Handles mouse unpress event
		void _on_move(wxMouseEvent &e);			///<Handles mouse move event
		void _on_wheel(wxMouseEvent &e);		///<Handles mouse wheel rotation event

	public:
		Mouse(Frame *frame) noexcept;			///<Creates mouse event handler
	};
}

#endif
