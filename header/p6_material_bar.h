#ifndef P6_MATERIAL_BAR
#define P6_MATERIAL_BAR

#include "../header/p6_frame.h"
#include <wx/wx.h>

namespace p6
{
	class MaterialBar
	{
	private:
		Frame *_frame;
		wxChoice *_name_choice;
		wxTextCtrl *_name_text;
		wxButton *_new_button;
		wxButton *_apply_button;
		wxButton *_delete_button;
		wxCheckBox *_nonlinear_check;
		wxTextCtrl *_formula_text;
		void OnMaterialChoice(wxCommandEvent &e);
		void OnMaterialName(wxCommandEvent &e);
		void OnMaterialNew(wxCommandEvent &e);
		void OnMaterialApply(wxCommandEvent &e);
		void OnMaterialDelete(wxCommandEvent &e);
		void OnMaterialNonlinear(wxCommandEvent &e);
		void OnMaterialFormula(wxCommandEvent &e);

	public:
		MaterialBar();
		void show();
	};
};

#endif