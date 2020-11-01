#ifndef P6_STICK_BAR
#define P6_STICK_BAR

#include <wx/wx.h>

namespace p6
{
	class StickBar
	{
		wxTextCtrl *_area_text;
		wxChoice *_material_choice;
		wxStaticText *_length_static;
		wxStaticText *_strain_static;
		wxStaticText *_force_static;
		void OnStickArea(wxCommandEvent &e);
		void OnStickMaterial(wxCommandEvent &e);

	};
};

#endif