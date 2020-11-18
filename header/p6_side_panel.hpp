/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_SIDE_PANEL
#define P6_SIDE_PANEL

#include "p6_node_bar.hpp"
#include "p6_stick_bar.hpp"
#include "p6_force_bar.hpp"
#include "p6_material_bar.hpp"
#include "p6_move_bar.hpp"
#include <wx/wx.h>

namespace p6
{
	class Frame;

	///Side panel is container that displays one of bars
	class SidePanel
	{
	private:
		///List of available bars
		enum class Bar
		{
			node,
			stick,
			force,
			move,
			material
		};

		Frame *_frame;						///<Application's window
		wxPanel *_panel;					///<wxWidget's panel
		wxBoxSizer *_sizer;					///<Sizer of wxWidget's panel
		NodeBar _node_bar;					///<Node bar
		StickBar _stick_bar;				///<Stick bar
		ForceBar _force_bar;				///<Force bar
		MaterialBar _material_bar;			///<Material bar
		MoveBar _move_bar;					///<Move bar
		Bar _bar = Bar::material;			///<Bar being displayed

		void _switch(Bar bar);				///<Show certain bar

	public:
		SidePanel(Frame *frame)		noexcept;	///<Creates side panel
		wxPanel *panel()			noexcept;	///<Returns wxWidget's panel
		wxBoxSizer *sizer()			noexcept;	///<Returns wxWidget's sizer
		MoveBar *move_bar()			noexcept;	///<Returns move bar
		void refresh()				noexcept;	///<Refreshes contents of bar's components, except choice boxes
		void refresh_materials()	noexcept;	///<Refreshes contents of choice boxes
	};
}

#endif