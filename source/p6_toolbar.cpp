/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_frame.hpp"
#include "../header/p6_utils.hpp"

void p6::ToolBar::_on_simulate(wxCommandEvent &e)
{
	if (tool == Tool::simulate)
	{
		_frame->construction.simulate(false);
		tool = Tool::no;
	}
	else
	{
		try
		{
			_frame->construction.simulate(true);
			tool = Tool::simulate;
		}
		catch (std::exception &e)
		{
			wxMessageBox(e.what(), "Error", wxICON_ERROR, _frame->frame);
		}
	}
}

void p6::ToolBar::_on_select(wxCommandEvent &e)
{
	if (tool == Tool::select) tool = Tool::no;
	else tool = Tool::select;
	_frame->main_panel.selected_nodes.clear();
	_frame->main_panel.selected_sticks.clear();
	_frame->main_panel.selected_forces.clear();
}

void p6::ToolBar::_on_area(wxCommandEvent &e)
{
	if (tool == Tool::area) tool = Tool::no;
	else tool = Tool::area;
	_frame->main_panel.selected_nodes.clear();
	_frame->main_panel.selected_sticks.clear();
	_frame->main_panel.selected_forces.clear();
}

void p6::ToolBar::_on_move(wxCommandEvent &e)
{
	if (tool == Tool::move) tool = Tool::no;
	else tool = Tool::move;
}

void p6::ToolBar::_on_node(wxCommandEvent &e)
{
	MainPanel *main = &_frame->main_panel;
	if (tool == Tool::node) tool = Tool::no;
	else if (!main->selected_nodes.empty()
		&& (!main->selected_sticks.empty() || !main->selected_sticks.empty()))
	{
		//Choose only nodes
		main->selected_sticks.clear();
		main->selected_sticks.clear();
	}
	else
	{
		//Turn node mode on
		tool = Tool::node;
		main->selected_nodes.clear();
		main->selected_sticks.clear();
		main->selected_forces.clear();
	}
}

void p6::ToolBar::_on_stick(wxCommandEvent &e)
{
	MainPanel *main = &_frame->main_panel;
	if (tool == Tool::stick) tool = Tool::no;
	else if (main->selected_nodes.size() == 2 && main->selected_sticks.empty() && main->selected_forces.empty())
	{
		//Connect two sticks
		uint node[2];
		node[0] = *main->selected_nodes.cbegin();
		node[1] = *++main->selected_nodes.cbegin();
		main->selected_nodes.clear();
		_frame->construction.create_stick(node, (uint)-1, 0.0);
	}
	else if (!main->selected_sticks.empty() && (!main->selected_nodes.empty() || !main->selected_forces.empty()))
	{
		//Choose only sticks
		main->selected_nodes.clear();
		main->selected_forces.clear();
	}
	else
	{
		//Turn stick mode on
		tool = Tool::stick;
		main->selected_nodes.clear();
		main->selected_sticks.clear();
		main->selected_forces.clear();
	}
}

void p6::ToolBar::_on_force(wxCommandEvent &e)
{
	MainPanel *main = &_frame->main_panel;
	if (tool == Tool::force) tool = Tool::no;
	else if (!main->selected_forces.empty() && (!main->selected_nodes.empty() || !main->selected_sticks.empty()))
	{
		//Choose only forces
		main->selected_nodes.clear();
		main->selected_sticks.clear();
	}
	else
	{
		//Turn force mode on
		tool = Tool::force;
		main->selected_nodes.clear();
		main->selected_sticks.clear();
		main->selected_forces.clear();
	}
}

void p6::ToolBar::_on_delete(wxCommandEvent &e)
{
	MainPanel *main = &_frame->main_panel;
	if (tool == Tool::delet) tool = Tool::no;
	else
	{
		//Delete forces
		for (auto i = main->selected_forces.cbegin(); i != main->selected_forces.cend(); i++)
			_frame->construction.delete_force(*i);

		//Delete selected sticks
		for (auto i = main->selected_sticks.cbegin(); i != main->selected_sticks.cend(); i++)
			_frame->construction.delete_stick(*i);

		//Delete nodes
		for (auto i = main->selected_nodes.cbegin(); i != main->selected_nodes.cend(); i++)
			_frame->construction.delete_node(*i);

		main->selected_nodes.clear();
		main->selected_sticks.clear();
		main->selected_forces.clear();
	}
}


p6::ToolBar::ToolBar(Frame *frame)
{
	wxToolBar *toolbar = frame->frame->CreateToolBar(wxID_ANY, wxID_ANY, "Toolbar");

	//Simulate tool
	_simulate = toolbar->AddCheckTool(
		wxID_ANY,
		"Simulate",
		Utils::load_png("icons/simulate.png"),
		Utils::load_png("icons/simulate_disabled.png"),
		"Run simulation",
		"Run simulation of the designed construction");
	frame->frame->Bind(wxEVT_TOOL, &ToolBar::_on_simulate, this, _simulate->GetId());

	//Select tool
	_select = toolbar->AddCheckTool(
		wxID_ANY,
		"Select",
		Utils::load_png("icons/select.png"),
		Utils::load_png("icons/select_disabled.png"),
		"Select items",
		"Select nodes, sticks or forces one by one");
	frame->frame->Bind(wxEVT_TOOL, &ToolBar::_on_select, this, _select->GetId());

	//Area tool
	_area = toolbar->AddCheckTool(
		wxID_ANY,
		"Area",
		Utils::load_png("icons/area.png"),
		Utils::load_png("icons/area_disabled.png"),
		"Select area",
		"Select all nodes, sticks or forces in area");
	frame->frame->Bind(wxEVT_TOOL, &ToolBar::_on_area, this, _area->GetId());

	//Move tool
	_move = toolbar->AddCheckTool(
		wxID_ANY,
		"Move",
		Utils::load_png("icons/move.png"),
		Utils::load_png("icons/move_disabled.png"),
		"Move nodes",
		"Complex move of selected nodes");
	frame->frame->Bind(wxEVT_TOOL, &ToolBar::_on_move, this, _move->GetId());

	//Node tool
	_node = toolbar->AddCheckTool(
		wxID_ANY,
		"Node",
		Utils::load_png("icons/node.png"),
		Utils::load_png("icons/node_disabled.png"),
		"Create node",
		"Create node or select nodes only");
	frame->frame->Bind(wxEVT_TOOL, &ToolBar::_on_node, this, _node->GetId());

	//Stick tool
	_stick = toolbar->AddCheckTool(
		wxID_ANY,
		"Stick",
		Utils::load_png("icons/stick.png"),
		Utils::load_png("icons/stick_disabled.png"),
		"Create stick",
		"Create stick between selected nodes or select sticks only");
	frame->frame->Bind(wxEVT_TOOL, &ToolBar::_on_stick, this, _stick->GetId());

	//Force tool
	_force = toolbar->AddCheckTool(
		wxID_ANY,
		"Force",
		Utils::load_png("icons/force.png"),
		Utils::load_png("icons/force_disabled.png"),
		"Create force",
		"Create force on selected node or select nodes only");
	frame->frame->Bind(wxEVT_TOOL, &ToolBar::_on_force, this, _force->GetId());

	//Delete tool
	_delete = toolbar->AddCheckTool(
		wxID_ANY,
		"Delete",
		Utils::load_png("icons/delete.png"),
		Utils::load_png("icons/delete_disabled.png"),
		"Delete",
		"Delete selected items");
	frame->frame->Bind(wxEVT_TOOL, &ToolBar::_on_delete, this, _delete->GetId());
	toolbar->Realize();
}