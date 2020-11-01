#ifndef P6_FORCE_BAR
#define P6_FORCE_BAR

#include <wx/wx.h>

namespace p6
{
	class ForceBar
	{
		wxTextCtrl *_x_text;
		wxTextCtrl *_y_text;
		void OnForceX(wxCommandEvent &e);
		void OnForceY(wxCommandEvent &e);

	};
};

#endif