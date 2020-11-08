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
		void _on_material_choice(wxCommandEvent &e);
		void _on_material_name(wxCommandEvent &e);
		void _on_material_new(wxCommandEvent &e);
		void _on_material_apply(wxCommandEvent &e);
		void _on_material_delete(wxCommandEvent &e);
		void _on_material_nonlinear(wxCommandEvent &e);
		void _on_material_formula(wxCommandEvent &e);

	public:
		MaterialBar(Frame *frame);
		void show();
		void refresh();
		void hide();
	};
};

#endif