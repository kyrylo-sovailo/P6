/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_STICK_BAR
#define P6_STICK_BAR

#include "p6_frame.hpp"
#include <wx/wx.h>

namespace p6
{
	class StickBar
	{
	private:
		Frame *_frame;
		wxStaticText *_material_static;
		wxChoice *_material_choice; 
		wxStaticText *_area_static;
		wxTextCtrl *_area_text;
		wxStaticText *_length_static;
		wxTextCtrl *_length_text;
		wxStaticText *_strain_static;
		wxTextCtrl *_strain_text;
		wxStaticText *_force_static;
		wxTextCtrl *_force_text;
		void _on_area(wxCommandEvent &e);
		void _on_material(wxCommandEvent &e);

	public:
		StickBar(Frame *frame);
		void show();
		void refresh();
		void refresh_material();
		void hide();
	};
}

#endif