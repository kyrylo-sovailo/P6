/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_toolbar.hpp"
#include "../header/p6_frame.hpp"
#include <wx/rawbmp.h>

void p6::ToolBar::_on_simulate(wxCommandEvent &e)
{
	if (_simulation)
	{
		_frame->construction()->simulate(false);
		_simulation = false;
	}
	else
	{
		try
		{
			_frame->construction()->simulate(true);
			_simulation = true;
		}
		catch (std::exception &e)
		{
			wxMessageBox(e.what(), "Error", wxICON_ERROR, _frame->frame());
		}
	}
	_frame->menubar()->refresh();
	_frame->main_panel()->need_refresh();
	_frame->side_panel()->refresh();
	_refresh();
}

void p6::ToolBar::_on_select(wxCommandEvent &e)
{
	if (_tool == Tool::select) _tool = Tool::no;
	else _tool = Tool::select;
	_frame->main_panel()->selected_nodes.clear();
	_frame->main_panel()->selected_sticks.clear();
	_frame->main_panel()->selected_forces.clear();
	_frame->main_panel()->need_refresh();
	_frame->side_panel()->refresh();
	_refresh();
}

void p6::ToolBar::_on_area(wxCommandEvent &e)
{
	if (_tool == Tool::area) _tool = Tool::no;
	else _tool = Tool::area;
	_frame->main_panel()->selected_nodes.clear();
	_frame->main_panel()->selected_sticks.clear();
	_frame->main_panel()->selected_forces.clear();
	_frame->main_panel()->need_refresh();
	_frame->side_panel()->refresh();
	_refresh();
}

void p6::ToolBar::_on_move(wxCommandEvent &e)
{
	if (_tool == Tool::move) _tool = Tool::no;
	else _tool = Tool::move;
	_refresh();
	_frame->main_panel()->need_refresh();
	_frame->side_panel()->refresh();
}

void p6::ToolBar::_on_node(wxCommandEvent &e)
{
	MainPanel *main = _frame->main_panel();
	if (_tool == Tool::node) _tool = Tool::no;
	else if (!main->selected_nodes.empty()
		&& (!main->selected_sticks.empty() || !main->selected_sticks.empty()))
	{
		//Choose only nodes
		main->selected_sticks.clear();
		main->selected_forces.clear();
		_frame->main_panel()->need_refresh();
		_frame->side_panel()->refresh();
	}
	else
	{
		//Turn node mode on
		_tool = Tool::node;
		main->selected_nodes.clear();
		main->selected_sticks.clear();
		main->selected_forces.clear();
		_frame->main_panel()->need_refresh();
		_frame->side_panel()->refresh();
	}
	_refresh();
}

void p6::ToolBar::_on_stick(wxCommandEvent &e)
{
	MainPanel *main = _frame->main_panel();
	if (_tool == Tool::stick) _tool = Tool::no;
	else if (main->selected_nodes.size() == 2 && main->selected_sticks.empty() && main->selected_forces.empty())
	{
		//Connect two sticks
		uint node[2];
		node[0] = *main->selected_nodes.cbegin();
		node[1] = *++main->selected_nodes.cbegin();
		main->selected_nodes.clear();
		_frame->construction()->create_stick(node, (uint)-1, 0.0);
		_frame->main_panel()->need_refresh();
		_frame->side_panel()->refresh();
	}
	else if (!main->selected_sticks.empty() && (!main->selected_nodes.empty() || !main->selected_forces.empty()))
	{
		//Choose only sticks
		main->selected_nodes.clear();
		main->selected_forces.clear();
		_frame->main_panel()->need_refresh();
		_frame->side_panel()->refresh();
	}
	else
	{
		//Turn stick mode on
		_tool = Tool::stick;
		main->selected_nodes.clear();
		main->selected_sticks.clear();
		main->selected_forces.clear();
		_frame->main_panel()->need_refresh();
		_frame->side_panel()->refresh();
	}
	_refresh();
}

void p6::ToolBar::_on_force(wxCommandEvent &e)
{
	MainPanel *main = _frame->main_panel();
	if (_tool == Tool::force) _tool = Tool::no;
	else if (!main->selected_forces.empty() && (!main->selected_nodes.empty() || !main->selected_sticks.empty()))
	{
		//Choose only forces
		main->selected_nodes.clear();
		main->selected_sticks.clear();
		_frame->main_panel()->need_refresh();
		_frame->side_panel()->refresh();
	}
	else
	{
		//Turn force mode on
		_tool = Tool::force;
		main->selected_nodes.clear();
		main->selected_sticks.clear();
		main->selected_forces.clear();
		_frame->main_panel()->need_refresh();
		_frame->side_panel()->refresh();
	}
	_refresh();
}

void p6::ToolBar::_on_delete(wxCommandEvent &e)
{
	MainPanel *main = _frame->main_panel();
	if (_tool == Tool::delet) _tool = Tool::no;
	else if (!main->selected_forces.empty() || !main->selected_nodes.empty() || !main->selected_sticks.empty())
	{
		//Delete forces
		for (auto i = main->selected_forces.crbegin(); i != main->selected_forces.crend(); i++)
			_frame->construction()->delete_force(*i);

		//Delete selected sticks
		for (auto i = main->selected_sticks.crbegin(); i != main->selected_sticks.crend(); i++)
			_frame->construction()->delete_stick(*i);

		//Delete nodes
		for (auto i = main->selected_nodes.crbegin(); i != main->selected_nodes.crend(); i++)
			_frame->construction()->delete_node(*i);

		main->selected_nodes.clear();
		main->selected_sticks.clear();
		main->selected_forces.clear();
		_frame->main_panel()->need_refresh();
		_frame->side_panel()->refresh();
	}
	else _tool = Tool::delet;
	_refresh();
}

wxBitmap p6::ToolBar::_load_png(const String filepath) noexcept
{
	wxLogNull lognull;
	wxImage image = wxImage(filepath, wxBITMAP_TYPE_PNG);
	if (!image.IsOk()) return wxBitmap(16, 16);
	
	wxColour colour = _toolbar->GetBackgroundColour();
	for (int x = 0; x < image.GetHeight(); x++)
		for (int y = 0; y < image.GetWidth(); y++)
		{
			if (image.GetAlpha(x, y) == 0)
			{
				image.SetAlpha(x, y, 255);
				image.SetRGB(x, y, colour.Red(), colour.Green(), colour.Blue());
			}
		}
	return wxBitmap(image);
}

void p6::ToolBar::_refresh() noexcept
{
	if (_simulation && (_tool == Tool::delet || _tool == Tool::move)) _tool = Tool::no;
	wxToolBar *toolbar = _frame->frame()->GetToolBar();
	toolbar->ToggleTool(_simulate->GetId(), _simulation);
	toolbar->ToggleTool(_select->GetId(), _tool == Tool::select);
	toolbar->ToggleTool(_area->GetId(), _tool == Tool::area);
	toolbar->ToggleTool(_move->GetId(), _tool == Tool::move);
	toolbar->EnableTool(_move->GetId(), !_simulation);
	toolbar->ToggleTool(_node->GetId(), _tool == Tool::node);
	toolbar->ToggleTool(_stick->GetId(), _tool == Tool::stick);
	toolbar->ToggleTool(_force->GetId(), _tool == Tool::force);
	toolbar->ToggleTool(_delete->GetId(), _tool == Tool::delet);
	toolbar->EnableTool(_delete->GetId(), !_simulation);
}

p6::ToolBar::ToolBar(Frame *frame) noexcept : _frame(frame)
{
	_toolbar = frame->frame()->CreateToolBar(wxID_ANY, wxID_ANY, "Toolbar");

	//Simulate tool
	_simulate = _toolbar->AddCheckTool(
		wxID_ANY,
		"Simulate",
		_load_png("icons/simulate.png"),
		wxNullBitmap,
		"Run simulation",
		"Run simulation of the designed construction");
	frame->frame()->Bind(wxEVT_TOOL, &ToolBar::_on_simulate, this, _simulate->GetId());

	//Select tool
	_select = _toolbar->AddCheckTool(
		wxID_ANY,
		"Select",
		_load_png("icons/select.png"),
		wxNullBitmap,
		"Select items",
		"Select nodes, sticks or forces one by one");
	frame->frame()->Bind(wxEVT_TOOL, &ToolBar::_on_select, this, _select->GetId());

	//Area tool
	_area = _toolbar->AddCheckTool(
		wxID_ANY,
		"Area",
		_load_png("icons/area.png"),
		wxNullBitmap,
		"Select area",
		"Select all nodes, sticks or forces in area");
	frame->frame()->Bind(wxEVT_TOOL, &ToolBar::_on_area, this, _area->GetId());

	//Move tool
	_move = _toolbar->AddCheckTool(
		wxID_ANY,
		"Move",
		_load_png("icons/move.png"),
		wxNullBitmap,
		"Move nodes",
		"Complex move of selected nodes");
	frame->frame()->Bind(wxEVT_TOOL, &ToolBar::_on_move, this, _move->GetId());

	//Node tool
	_node = _toolbar->AddCheckTool(
		wxID_ANY,
		"Node",
		_load_png("icons/node.png"),
		wxNullBitmap,
		"Create node",
		"Create node or select nodes only");
	frame->frame()->Bind(wxEVT_TOOL, &ToolBar::_on_node, this, _node->GetId());

	//Stick tool
	_stick = _toolbar->AddCheckTool(
		wxID_ANY,
		"Stick",
		_load_png("icons/stick.png"),
		wxNullBitmap,
		"Create stick",
		"Create stick between selected nodes or select sticks only");
	frame->frame()->Bind(wxEVT_TOOL, &ToolBar::_on_stick, this, _stick->GetId());

	//Force tool
	_force = _toolbar->AddCheckTool(
		wxID_ANY,
		"Force",
		_load_png("icons/force.png"),
		wxNullBitmap,
		"Create force",
		"Create force on selected node or select nodes only");
	frame->frame()->Bind(wxEVT_TOOL, &ToolBar::_on_force, this, _force->GetId());

	//Delete tool
	_delete = _toolbar->AddCheckTool(
		wxID_ANY,
		"Delete",
		_load_png("icons/delete.png"),
		wxNullBitmap,
		"Delete",
		"Delete selected items");
	frame->frame()->Bind(wxEVT_TOOL, &ToolBar::_on_delete, this, _delete->GetId());
	_toolbar->Realize();
}

p6::ToolBar::Tool p6::ToolBar::tool() const noexcept
{
	return _tool;
}

bool p6::ToolBar::simulation() const noexcept
{
	return _simulation;
}