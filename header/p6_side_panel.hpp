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

	class SidePanel
	{
	private:
		enum class Mode
		{
			node,
			stick,
			force,
			move,
			material
		};

		Frame *_frame;
		wxPanel *_panel;
		wxBoxSizer *_sizer;
		NodeBar _node_bar;
		StickBar _stick_bar;
		ForceBar _force_bar;
		MaterialBar _material_bar;
		MoveBar _move_bar;
		Mode _mode = Mode::material;

		void _switch(Mode mode);

	public:
		SidePanel(Frame *frame);
		wxPanel *panel();
		wxBoxSizer *sizer();
		void refresh();
		void refresh_materials();
	};
}

#endif