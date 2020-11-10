/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_mouse.hpp"
#include "../header/p6_frame.hpp"

void p6::Mouse::_on_left_down(wxMouseEvent &e)
{
	_pressed_item = _frame->main_panel()->get_item(e.GetPosition());
	_pressed_point = e.GetPosition();
	_pressed = true;
	_moving = false;
}

void p6::Mouse::_on_left_up(wxMouseEvent &e)
{
	if (!_moving && _pressed_item.type == MainPanel::Item::Type::node)
	{
		//Select node
		std::set<uint> *selected_nodes = &_frame->main_panel()->selected_nodes;
		if (!selected_nodes->count(_pressed_item.index))
		{
			selected_nodes->insert(_pressed_item.index);
			if (_frame->toolbar()->tool() == ToolBar::Tool::stick && selected_nodes->size() == 2)
			{
				//Create stick (in stick mode i selected 2 nodes)
				uint node[2];
				node[0] = *selected_nodes->cbegin();
				node[1] = *++selected_nodes->cbegin();
				_frame->construction()->create_stick(node, (uint)-1, 0.0);
				selected_nodes->clear();
			}
		}
		else selected_nodes->erase(_pressed_item.index);
		_frame->main_panel()->need_refresh();
		_frame->side_panel()->refresh();
	}
	else if (!_moving && _pressed_item.type == MainPanel::Item::Type::stick)
	{
		//Select stick
		std::set<uint> *selected_sticks = &_frame->main_panel()->selected_sticks;
		if (selected_sticks->count(_pressed_item.index)) selected_sticks->erase(_pressed_item.index);
		else selected_sticks->insert(_pressed_item.index);
		_frame->main_panel()->need_refresh();
		_frame->side_panel()->refresh();
	}
	else if (!_moving && _pressed_item.type == MainPanel::Item::Type::force)
	{
		//Select force
		std::set<uint> *selected_forces = &_frame->main_panel()->selected_forces;
		if (selected_forces->count(_pressed_item.index)) selected_forces->erase(_pressed_item.index);
		else selected_forces->insert(_pressed_item.index);
		_frame->main_panel()->need_refresh();
		_frame->side_panel()->refresh();
	}
	else if (_moving && _frame->toolbar()->tool() == ToolBar::Tool::area)
	{
		//Finish area selection
		_frame->main_panel()->selected_area_points[1] = e.GetPosition();
		_frame->main_panel()->select_items(); 
		_frame->main_panel()->selected_area_draw = false;
		_frame->main_panel()->need_refresh();
		_frame->side_panel()->refresh();
	}
	else if (_moving && _pressed_item.type == MainPanel::Item::Type::force && _frame->toolbar()->tool() == ToolBar::Tool::force)
	{
		//Finish force creation
		MainPanel *main_panel = _frame->main_panel();
		Construction *con = _frame->construction();
		Coord point_coord = main_panel->pixel_to_real(e.GetPosition());
		Coord node_coord = con->get_node_coord(con->get_force_node(_pressed_item.index));
		Coord direction = (point_coord - node_coord) / main_panel->meters_in_newton;
		_frame->construction()->set_force_direction(_pressed_item.index, direction);
		_frame->main_panel()->need_refresh();
	}
	else if (_moving && _pressed_item.type == MainPanel::Item::Type::node)
	{
		//Finish node dragging
		_frame->construction()->set_node_coord(_pressed_item.index, _frame->main_panel()->pixel_to_real(e.GetPosition()));
		_frame->main_panel()->need_refresh();
		_frame->side_panel()->refresh();		//Theoretically doesn't need to refresh for example force bar
	}
	else if (_moving)
	{
		//Finish dragging view
		MainPanel *main_panel = _frame->main_panel();
		main_panel->center.x = _old_center.x + (_pressed_point.x - e.GetX()) / main_panel->pixels_in_meter;
		main_panel->center.y = _old_center.y + (e.GetX() - _pressed_point.x) / main_panel->pixels_in_meter;
		_frame->main_panel()->need_refresh();
	}
	_pressed = false;
}

void p6::Mouse::_on_move(wxMouseEvent &e)
{
	if (!_pressed) return;
	
	_moving = true;
	if (_frame->toolbar()->tool() == ToolBar::Tool::area)
	{
		//Area selection
		_frame->main_panel()->selected_area_points[0] = _pressed_point;
		_frame->main_panel()->selected_area_points[1] = e.GetPosition();
		_frame->main_panel()->selected_area_draw = true;
		_frame->main_panel()->need_refresh();
	}
	else if (_pressed_item.type == MainPanel::Item::Type::node && _frame->toolbar()->tool() == ToolBar::Tool::force)
	{
		//Force creation
		MainPanel *main_panel = _frame->main_panel(); 
		Construction *con = _frame->construction();
		Coord point_coord = main_panel->pixel_to_real(e.GetPosition());
		Coord node_coord = con->get_node_coord(_pressed_item.index);
		Coord direction = (point_coord - node_coord) / main_panel->meters_in_newton;
		_pressed_item.type = MainPanel::Item::Type::force;
		_pressed_item.index = con->create_force(_pressed_item.index, direction);
		_frame->main_panel()->need_refresh();
	}
	else if (_pressed_item.type == MainPanel::Item::Type::node && _frame->toolbar()->tool() != ToolBar::Tool::simulate)
	{
		//Node dragging
		_frame->construction()->set_node_coord(_pressed_item.index, _frame->main_panel()->pixel_to_real(e.GetPosition()));
		_frame->main_panel()->need_refresh();
		_frame->side_panel()->refresh();		//Theoretically doesn't need to refresh for example force bar
	}
	else
	{
		//View dragging
		MainPanel *main_panel = _frame->main_panel();
		main_panel->center.x = _old_center.x + (_pressed_point.x - e.GetX()) / main_panel->pixels_in_meter;
		main_panel->center.y = _old_center.y + (e.GetX() - _pressed_point.x) / main_panel->pixels_in_meter;
		_frame->main_panel()->need_refresh();
	}
	_moving = true;
}

void p6::Mouse::_on_wheel(wxMouseEvent &e)
{
	if (_pressed) return;

	if (_frame->toolbar()->tool() == ToolBar::Tool::force)
	{
		if (e.GetWheelRotation() > 0) _wheel_force++;
		else if (e.GetWheelRotation() < 0) _wheel_force--;
		_frame->main_panel()->meters_in_newton = exp2(_wheel_force);
	}
	else
	{
		if (e.GetWheelRotation() > 0) _wheel++;
		else if (e.GetWheelRotation() < 0) _wheel--;
		_frame->main_panel()->pixels_in_meter = 30.0 * exp2(_wheel);
	}
	_frame->main_panel()->need_refresh();
}

p6::Mouse::Mouse(Frame *frame)
{
	_frame = frame;
	wxWindow *parent = frame->main_panel()->panel();
	parent->Bind(wxEVT_LEFT_DOWN, &Mouse::_on_left_down, this, parent->GetId());
	parent->Bind(wxEVT_LEFT_UP, &Mouse::_on_left_up, this, parent->GetId());
	parent->Bind(wxEVT_MOTION, &Mouse::_on_move, this, parent->GetId());
	parent->Bind(wxEVT_MOUSEWHEEL, &Mouse::_on_wheel, this, wxID_ANY);
}