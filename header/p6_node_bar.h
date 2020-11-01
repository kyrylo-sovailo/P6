#ifndef P6_NODE_BAR
#define P6_NODE_BAR

#include <wx/wx.h>

namespace p6
{
	class NodeBar
	{
		wxCheckBox *_free_check;
		wxCheckBox *_fixed_check;
		wxTextCtrl *_x_text;
		wxTextCtrl *_y_text;
		void OnNodeFree(wxCommandEvent &e);
		void OnNodeFixed(wxCommandEvent &e);
		void OnNodeX(wxCommandEvent &e);
		void OnNodeY(wxCommandEvent &e);

	};
};

#endif