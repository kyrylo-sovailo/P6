/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_frame.hpp"

void p6::SidePanel::_switch(Mode mode)
{
	if (_mode == mode) return;

	//Hiding
	switch (_mode)
	{
	case Mode::node:
		_node_bar.hide();
		break;

	case Mode::stick:
		_stick_bar.hide();
		break;

	case Mode::force:
		_force_bar.hide();
		break;

	case Mode::move:
		_move_bar.hide();
		break;

	default:
		_material_bar.hide();
		break;
	}

	//Showing
	switch (mode)
	{
	case Mode::node:
		_node_bar.show();
		break;

	case Mode::stick:
		_stick_bar.show();
		break;

	case Mode::force:
		_force_bar.show();
		break;

	case Mode::move:
		_move_bar.show();
		break;

	default:
		_material_bar.show();
		break;
	}
	_mode = mode;
}

p6::SidePanel::SidePanel(Frame *frame) :
	_frame(frame),
	panel(new wxPanel(frame->frame, wxID_ANY)),
	sizer(new wxBoxSizer(wxHORIZONTAL)),
	_node_bar(frame),
	_stick_bar(frame),
	_force_bar(frame),
	_material_bar(frame),
	_move_bar(frame)
{
	panel->Show(false);
	sizer->Add(panel, 4, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN, 0);
	_material_bar.show();
}

void p6::SidePanel::refresh()
{
	const std::set<uint> *selected_nodes = &_frame->main_panel.selected_nodes;
	const std::set<uint> *selected_sticks = &_frame->main_panel.selected_sticks;
	const std::set<uint> *selected_forces = &_frame->main_panel.selected_forces;
	if (_frame->toolbar.tool == ToolBar::Tool::move)
	{
		_switch(Mode::move);
	}
	else if (!selected_nodes->empty() && selected_sticks->empty() && selected_forces->empty())
	{
		_switch(Mode::node);
	}
	else if (!selected_sticks->empty() && selected_nodes->empty() && selected_forces->empty())
	{
		_switch(Mode::stick);
	}
	else if (!selected_forces->empty() && selected_nodes->empty() && selected_sticks->empty())
	{
		_switch(Mode::force);
	}
	else
	{
		_switch(Mode::material);
	}
}