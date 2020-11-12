/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_MATERIAL_BAR
#define P6_MATERIAL_BAR

#include <wx/wx.h>

namespace p6
{
	class Frame;

	class MaterialBar
	{
	private:
		Frame *_frame;
		wxChoice *_material_choice;
		wxStaticText *_name_static;
		wxTextCtrl *_name_text;
		wxCheckBox *_nonlinear_check;
		wxStaticText *_formula_static;
		wxTextCtrl *_formula_text;
		wxBoxSizer *_buttons_sizer;
		wxButton *_button_new;
		wxButton *_button_apply;
		wxButton *_button_delete;

		void _on_choice(wxCommandEvent &e);
		void _on_new(wxCommandEvent &e);
		void _on_apply(wxCommandEvent &e);
		void _on_delete(wxCommandEvent &e);

	public:
		MaterialBar(Frame *frame);
		void show();
		void refresh();
		void refresh_materials();
		void hide();
	};
}

#endif