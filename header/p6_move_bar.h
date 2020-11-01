#ifndef P6_MOVE_BAR
#define P6_MOVE_BAR

#include <wx/wx.h>

namespace p6
{
	class MoveBar
	{
		wxTextCtrl *_anchor_x_text;
		wxTextCtrl *_anchor_y_text;
		wxTextCtrl *_scale_text;
		wxTextCtrl *_angle_text;
		wxTextCtrl *_shift_x_text;
		wxTextCtrl *_shift_y_text;
		void OnMoveAnchorX(wxCommandEvent &e);
		void OnMoveAnchorY(wxCommandEvent &e);
		void OnMoveScale(wxCommandEvent &e);
		void OnMoveAngle(wxCommandEvent &e);
		void OnMoveShiftX(wxCommandEvent &e);
		void OnMoveShiftY(wxCommandEvent &e);
	};
};

#endif