/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_MOVE_BAR
#define P6_MOVE_BAR

#include "p6_common.hpp"
#include <wx/wx.h>

namespace p6
{
	class Frame;

	///Force bar is used to move many nodes and forces at one time, one of possible contents of side bar
	class MoveBar
	{
	private:
		Frame *_frame;									///<Application's window
		wxStaticText *_anchor_x_static;					///<Text explaining anchor's X coordinate edit box
		wxTextCtrl *_anchor_x_text;						///<Anchor's X coordinate edit box
		wxStaticText *_anchor_y_static;					///<Text explaining anchor's Y coordinate edit box
		wxTextCtrl *_anchor_y_text;						///<Anchor's Y coordinate edit box
		wxStaticText *_scale_static;					///<Text explaining scale rate edit box
		wxTextCtrl *_scale_text;						///<Scale rate edit box
		wxStaticText *_angle_static;					///<Text explaining rotation angle edit box
		wxTextCtrl *_angle_text;						///<Rotation angle (in grad) edit box
		wxStaticText *_shift_x_static;					///<Text explaining horizontal shift edit box
		wxTextCtrl *_shift_x_text;						///<Horizontal shift edit box
		wxStaticText *_shift_y_static;					///<Text explaining vertical shift edit box
		wxTextCtrl *_shift_y_text;						///<Vertical shift edit box
		wxCheckBox *_allowed_shift;						///<Check box that decides if mouse is allowed to shift selection
		wxCheckBox *_allowed_scale;						///<Check box that decides if mouse is allowed to scale selection
		wxCheckBox *_allowed_rotate;					///<Check box that decides if mouse is allowed to rotate selection
		real _scale;									///<Scale rate
		real _angle;									///<Rotation angle (in rad)
		Coord _shift;									///<Shift
		Coord _initial_anchor;							///<Anchor's initial (without shift) position
		std::vector<Coord> _initial_node_coords;		///<Selected node's initial coordinates
		std::vector<Coord> _initial_force_directions;	///<Selected force's initial directions
		
		void _on_anchor_x(wxCommandEvent &e);			///<Handles text input in anchor's X coordinate edit box, changes anchor's X coordinate, resets scale rate and angle
		void _on_anchor_y(wxCommandEvent &e);			///<Handles text input in anchor's Y coordinate edit box, changes anchor's Y coordinate, resets scale rate and angle
		void _on_scale(wxCommandEvent &e);				///<Handles text input in scale rate edit box, changes scale rate
		void _on_angle(wxCommandEvent &e);				///<Handles text input in rotation angle edit box, changes rotation angle
		void _on_shift_x(wxCommandEvent &e);			///<Handles text input in horizontal shift edit box, changes horizontal shift
		void _on_shift_y(wxCommandEvent &e);			///<Handles text input in vertical shift edit box, changes vertical shift
		void _on_allowed_shift(wxCommandEvent &e);		///<Handles checking "shifting allowed" box
		void _on_allowed_scale(wxCommandEvent &e);		///<Handles checking "scaling allowed" box
		void _on_allowed_rotate(wxCommandEvent &e);		///<Handles checking "rotation allowed" box
		void _refresh_scale_angle()		noexcept;		///<Sets scale rate to one and angle to zero
		void _refresh_construction()	noexcept;		///<Calculates coordinates of selected nodes and directions of selected forces

	public:
		MoveBar(Frame *frame)			noexcept;		///<Creates move bar
		Coord anchor()					const noexcept;	///<Returns coordinates of anchor
		void set_anchor(Coord coord)	noexcept;		///<Sets coordinates of anchor
		void drag_begin(Coord coord)	noexcept;		///<Begins dragging by mouse
		void drag_continue(Coord coord)	noexcept;		///<Processes dragging by mouse
		void drag_end(Coord coord)		noexcept;		///<Ends dragging by mouse
		void show()						noexcept;		///<Adds bar's components to side bar
		void hide()						noexcept;		///<Removes bar's components from side bar
	};
}

#endif