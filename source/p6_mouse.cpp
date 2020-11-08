#include "../header/p6_frame.h"

void p6::Mouse::_on_left_down(wxMouseEvent &e)
{
	real x, y;
	_frame->main_panel.pixel_to_real(e.GetPosition(), &x, &y);
	selected_item = _frame->main_panel.get_item(x, y, &selected_index);
};

void p6::Mouse::_on_left_up(wxMouseEvent &e)
{
	if (moving)
	{
		if (selected_item == MainPanel::Item::node)
		{
			real x, y;
			_frame->main_panel.pixel_to_real(e.GetPosition(), &x, &y);
			_frame->construction.set_node_x(selected_index, x);
			_frame->construction.set_node_y(selected_index, y);
		}
		else if (_frame->toolbar.tool == ToolBar::Tool::area)
		{
			point_up = e.GetPosition();
			real ax, ay, bx, by;
			_frame->main_panel.pixel_to_real(point_down, &ax, &ay);
			_frame->main_panel.pixel_to_real(point_up, &bx, &by);
			_frame->main_panel.select_area(ax, ay, bx, by);
		}
		else
		{
			point_up = e.GetPosition();
			_frame->main_panel.center_x = _old_x + (point_down.x - point_up.x) / _frame->main_panel.pixels_in_meter;
			_frame->main_panel.center_y = _old_y + (point_up.y - point_down.y) / _frame->main_panel.pixels_in_meter;
		}
	}
	else
	{
		if (selected_item == MainPanel::Item::node)
		{
			if (_frame->main_panel.selected_nodes.count(selected_index))
				_frame->main_panel.selected_nodes.erase(selected_index);
			else
				_frame->main_panel.selected_nodes.insert(selected_index);
		}
		else if (selected_item == MainPanel::Item::node)
		{
			if (_frame->main_panel.selected_sticks.count(selected_index))
				_frame->main_panel.selected_sticks.erase(selected_index);
			else
				_frame->main_panel.selected_sticks.insert(selected_index);
		}
		else if (selected_item == MainPanel::Item::node)
		{
			if (_frame->main_panel.selected_forces.count(selected_index))
				_frame->main_panel.selected_forces.erase(selected_index);
			else
				_frame->main_panel.selected_forces.insert(selected_index);
		}
	}
};

void p6::Mouse::_on_right_down(wxMouseEvent &e)
{};

void p6::Mouse::_on_right_up(wxMouseEvent &e)
{};

void p6::Mouse::_on_move(wxMouseEvent &e)
{

};

void p6::Mouse::_on_wheel(wxMouseEvent &e)
{
	if (_frame->toolbar.tool == ToolBar::Tool::force)
	{
		if (e.GetWheelRotation() > 0) _wheel_force++;
		else if (e.GetWheelRotation() < 0) _wheel_force--;
		_frame->main_panel.meters_in_newton = exp2(_wheel_force);
	}
	else
	{
		if (e.GetWheelRotation() > 0) _wheel++;
		else if (e.GetWheelRotation() < 0) _wheel--;
		_frame->main_panel.pixels_in_meter = 30.0 * exp2(_wheel);
	}
};

p6::Mouse::Mouse(Frame *frame)
{
	_frame = frame;
	wxWindow *parent = frame->main_panel.panel;
	parent->Bind(wxEVT_LEFT_DOWN, &Mouse::_on_left_down, this, parent->GetId());
	parent->Bind(wxEVT_LEFT_UP, &Mouse::_on_left_up, this, parent->GetId());
	parent->Bind(wxEVT_RIGHT_DOWN, &Mouse::_on_right_down, this, parent->GetId());
	parent->Bind(wxEVT_RIGHT_UP, &Mouse::_on_right_up, this, parent->GetId());
	parent->Bind(wxEVT_MOTION, &Mouse::_on_move, this, parent->GetId());
	parent->Bind(wxEVT_MOUSEWHEEL, &Mouse::_on_wheel, this, wxID_ANY);
};