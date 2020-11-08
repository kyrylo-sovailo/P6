/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_FORCE_BAR
#define P6_FORCE_BAR

#include <wx/wx.h>

namespace p6
{
	class Frame;

	class ForceBar
	{
	private:
		Frame *_frame;
		wxStaticText *_x_static;
		wxTextCtrl *_x_text;
		wxStaticText *_y_static;
		wxTextCtrl *_y_text;
		void _on_force_x(wxCommandEvent &e);
		void _on_force_y(wxCommandEvent &e);

	public:
		ForceBar(Frame *frame);
		void show();
		void refresh();
		void hide();
	};
}

#endif