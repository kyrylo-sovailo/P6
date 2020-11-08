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
};

#endif