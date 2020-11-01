#ifndef P6_MOVE_BAR
#define P6_MOVE_BAR

#include <wx/wx.h>
#include "p6_common.h"

namespace p6
{
	struct Mouse
	{
		enum class State
		{
			unpressed,
			pressed_on_panel,
			pressed_on_node,
			pressed_on_stick,
			pressed_on_force,
			move_on_panel,
			move_on_node
		};

		State _state;		//Mouse state
		size_t _index;		//Index of selected element
		real _old_x;		//Old _x, used for dragging
		real _old_y;		//Old _y, used for dragging
		wxPoint _down;		//Point on main panel where mouse were pressed
		wxPoint _up;		//Point on main panel where mouse were unpressed
		int wheel;			//State of scaling wheel
		int wheel_force;	//State of force scaling wheel

		void OnLeftDown(wxMouseEvent &e);
		void OnLeftUp(wxMouseEvent &e);
		void OnRightDown(wxMouseEvent &e);
		void OnRightUp(wxMouseEvent &e);
		void OnMove(wxMouseEvent &e);
		void OnWheel(wxMouseEvent &e);
	};
};

#endif