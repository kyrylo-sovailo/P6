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

#include <wx/wx.h>

namespace p6
{
	class Frame;

	class MoveBar
	{
	private:
		Frame *_frame;
		wxTextCtrl *_anchor_x_text;
		wxTextCtrl *_anchor_y_text;
		wxTextCtrl *_scale_text;
		wxTextCtrl *_angle_text;
		wxTextCtrl *_shift_x_text;
		wxTextCtrl *_shift_y_text;

		void _on_anchor_x(wxCommandEvent &e);
		void _on_anchor_y(wxCommandEvent &e);
		void _on_scale(wxCommandEvent &e);
		void _on_angle(wxCommandEvent &e);
		void _on_shift_x(wxCommandEvent &e);
		void _on_shift_y(wxCommandEvent &e);

	public:
		MoveBar(Frame *frame);
		void show();
		void refresh();
		void hide();

	};
}

#endif