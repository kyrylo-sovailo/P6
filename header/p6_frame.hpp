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
	///Application's window
	class Frame
	{
	private:
		wxFrame *_frame;							///<wxWidget's window
		wxBoxSizer *_sizer;							///<Sizer of wxWidget's window
		ToolBar _toolbar;							///<Tool bar
		MenuBar _menubar;							///<Menu bar
		MainPanel _main_panel;						///<Main panel
		SidePanel _side_panel;						///<Side panel
		Mouse _mouse;								///<Mouse events' handler
		Construction _construction;					///<Construction being edited

		void _on_paint(wxPaintEvent &e);			///<Handles paint event through calling p6::MainPanel::render()
		void _on_size(wxSizeEvent &e);				///<Handles resize event

	public:
		Frame()							noexcept;	///<Creates window and it's components
		wxFrame *frame()				noexcept;	///<Returns wxWidget's window
		wxBoxSizer *sizer()				noexcept;	///<Returns sizer of wxWidget's window
		ToolBar *toolbar()				noexcept;	///<Returns tool bar
		MenuBar *menubar()				noexcept;	///<Returns menu bar
		MainPanel *main_panel()			noexcept;	///<Returns main panel
		SidePanel *side_panel()			noexcept;	///<Returns side panel
		Construction *construction()	noexcept;	///<Returns construction being edited
	};
}

#endif