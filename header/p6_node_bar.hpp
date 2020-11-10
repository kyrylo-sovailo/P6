/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_NODE_BAR
#define P6_NODE_BAR

#include <wx/wx.h>

namespace p6
{
	class Frame;

	class NodeBar
	{
	private:
		Frame *_frame;
		wxCheckBox *_free_check;
		wxCheckBox *_fixed_check;
		wxStaticText *_x_static;
		wxTextCtrl *_x_text;
		wxStaticText *_y_static;
		wxTextCtrl *_y_text;

		void _on_free(wxCommandEvent &e);
		void _on_fixed(wxCommandEvent &e);
		void _on_x(wxCommandEvent &e);
		void _on_y(wxCommandEvent &e);

	public:
		NodeBar(Frame *frame);
		void show();
		void refresh();
		void hide();
	};
}

#endif