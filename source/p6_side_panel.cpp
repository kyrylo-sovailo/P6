/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_side_panel.hpp"
#include "../header/p6_frame.hpp"

void p6::SidePanel::_switch(Bar bar)
{
	if (_bar == bar)
	{
		//Refreshing
		switch (bar)
		{
		case Bar::node:
			_node_bar.refresh();
			break;

		case Bar::stick:
			_stick_bar.refresh();
			break;

		case Bar::force:
			_force_bar.refresh();
			break;

		case Bar::move:
			_move_bar.refresh();
			break;

		default:
			_material_bar.refresh();
			break;
		}
	}
	else
	{
		//Hiding
		switch (_bar)
		{
		case Bar::node:
			_node_bar.hide();
			break;

		case Bar::stick:
			_stick_bar.hide();
			break;

		case Bar::force:
			_force_bar.hide();
			break;

		case Bar::move:
			_move_bar.hide();
			break;

		default:
			_material_bar.hide();
			break;
		}

		//Showing
		switch (bar)
		{
		case Bar::node:
			_node_bar.refresh();
			_node_bar.show();
			break;

		case Bar::stick:
			_stick_bar.refresh();
			_stick_bar.show();
			break;

		case Bar::force:
			_force_bar.refresh();
			_force_bar.show();
			break;

		case Bar::move:
			_move_bar.refresh();
			_move_bar.show();
			break;

		default:
			_material_bar.refresh();
			_material_bar.show();
			break;
		}
		_bar = bar;
	}
}

p6::SidePanel::SidePanel(Frame *frame) noexcept :
	_frame(frame),
	_panel(new wxPanel(frame->frame(), wxID_ANY)),
	_sizer(new wxBoxSizer(wxVERTICAL)),
	_node_bar(frame),
	_stick_bar(frame),
	_force_bar(frame),
	_material_bar(frame),
	_move_bar(frame)
{
	frame->sizer()->Add(_panel, 1, wxEXPAND, 0);
	_panel->SetSizer(_sizer);
	_material_bar.show();
}

wxPanel *p6::SidePanel::panel() noexcept
{
	return _panel;
}

wxBoxSizer *p6::SidePanel::sizer() noexcept
{
	return _sizer;
}

p6::MoveBar *p6::SidePanel::move_bar() noexcept
{
	return &_move_bar;
}

void p6::SidePanel::refresh() noexcept
{
	const std::set<uint> *selected_nodes = &_frame->main_panel()->selected_nodes;
	const std::set<uint> *selected_sticks = &_frame->main_panel()->selected_sticks;
	const std::set<uint> *selected_forces = &_frame->main_panel()->selected_forces;
	if (_frame->toolbar()->tool() == ToolBar::Tool::move)
	{
		_switch(Bar::move);
	}
	else if (!selected_nodes->empty() && selected_sticks->empty() && selected_forces->empty())
	{
		_switch(Bar::node);
	}
	else if (!selected_sticks->empty() && selected_nodes->empty() && selected_forces->empty())
	{
		_switch(Bar::stick);
	}
	else if (!selected_forces->empty() && selected_nodes->empty() && selected_sticks->empty())
	{
		_switch(Bar::force);
	}
	else
	{
		_switch(Bar::material);
	}
}

void p6::SidePanel::refresh_materials() noexcept
{
	_stick_bar.refresh_materials();
	_material_bar.refresh_materials();
	_panel->Layout();
}