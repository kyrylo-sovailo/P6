/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_FRAME
#define P6_FRAME

#include "p6_menubar.hpp"
#include "p6_toolbar.hpp"
#include "p6_main_panel.hpp"
#include "p6_side_panel.hpp"
#include "p6_mouse.hpp"
#include "p6_construction.hpp"
#include <wx/wx.h>

namespace p6
{
	class Frame
	{
	private:
		wxFrame *_frame;
		wxBoxSizer *_sizer;
		ToolBar _toolbar;
		MenuBar _menubar;
		MainPanel _main_panel;
		SidePanel _side_panel;
		Mouse _mouse;
		Construction _construction;

		void _on_paint(wxPaintEvent &e);
		void _on_size(wxSizeEvent &e);

	public:
		Frame();
		wxFrame *frame();
		wxBoxSizer *sizer();
		ToolBar *toolbar();
		MenuBar *menubar();
		MainPanel *main_panel();
		SidePanel *side_panel();
		Mouse *mouse();
		Construction *construction();
	};
}

#endif