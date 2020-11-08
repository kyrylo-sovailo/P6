#ifndef P6_FRAME
#define P6_FRAME

#include "p6_menubar.h"
#include "p6_toolbar.h"
#include "p6_main_panel.h"
#include "p6_side_panel.h"
#include "p6_mouse.h"
#include "p6_construction.h"
#include <wx/wx.h>

namespace p6
{
	class Frame
	{
	private:
		void _on_paint(wxPaintEvent &e);
		void _on_size(wxSizeEvent &e);

	public:
		wxFrame *frame;
		wxBoxSizer *sizer;
		ToolBar toolbar;
		MenuBar menubar;
		MainPanel main_panel;
		SidePanel side_panel;
		Mouse mouse;
		Construction construction;
		Frame();
	};
};

#endif