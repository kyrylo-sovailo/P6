/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_move_bar.hpp"
#include "../header/p6_frame.hpp"

void p6::MoveBar::_on_anchor_x(wxCommandEvent &e)
{
	real x = string_to_real(_anchor_x_text->GetValue().ToStdString());
	if (x == x)
	{
		_refresh_scale_angle();
		_initial_anchor.x = x - _shift.x;
		_frame->main_panel()->need_refresh();
	}
}

void p6::MoveBar::_on_anchor_y(wxCommandEvent &e)
{
	real y = string_to_real(_anchor_y_text->GetValue().ToStdString());
	if (y == y)
	{
		_refresh_scale_angle();
		_initial_anchor.y = y - _shift.y;
		_frame->main_panel()->need_refresh();
	}
}

void p6::MoveBar::_on_scale(wxCommandEvent &e)
{
	real s = string_to_real(_scale_text->GetValue().ToStdString());
	if (s == s)
	{
		_scale = s;
		_refresh_construction();
	}
}

void p6::MoveBar::_on_angle(wxCommandEvent &e)
{
	real a = string_to_real(_angle_text->GetValue().ToStdString());
	if (a == a)
	{
		_angle = pi() * a / 180.0;
		_refresh_construction();
	}
}

void p6::MoveBar::_on_shift_x(wxCommandEvent &e)
{
	real x = string_to_real(_shift_x_text->GetValue().ToStdString());
	if (x == x)
	{
		_shift.x = x;
		_anchor_x_text->ChangeValue(real_to_string(_initial_anchor.x + _shift.x));
		_refresh_construction();
	}
}

void p6::MoveBar::_on_shift_y(wxCommandEvent &e)
{
	real y = string_to_real(_shift_y_text->GetValue().ToStdString());
	if (y == y)
	{
		_shift.y = y;
		_anchor_y_text->ChangeValue(real_to_string(_initial_anchor.y + _shift.y));
		_refresh_construction();
	}
}

void p6::MoveBar::_on_allowed_shift(wxCommandEvent &e)
{
	if (_allowed_shift->GetValue())
	{
		_allowed_scale->SetValue(false);
		_allowed_rotate->SetValue(false);
	}
}

void p6::MoveBar::_on_allowed_scale(wxCommandEvent &e)
{
	if (_allowed_scale->GetValue())
	{
		_allowed_shift->SetValue(false);
		_allowed_rotate->SetValue(false);
	}
}

void p6::MoveBar::_on_allowed_rotate(wxCommandEvent &e)
{
	if (_allowed_rotate->GetValue())
	{
		_allowed_shift->SetValue(false);
		_allowed_scale->SetValue(false);
	}
}

void p6::MoveBar::_refresh_scale_angle() noexcept
{
	for (uint i = 0; i < _initial_node_coords.size(); i++)
	{
		Coord relative_coord = _initial_node_coords[i] - _initial_anchor;
		_initial_node_coords[i] = _initial_anchor + relative_coord.rotate(_angle) * _scale;
	}

	for (uint i = 0; i < _initial_force_directions.size(); i++)
	{
		_initial_force_directions[i] = _initial_force_directions[i].rotate(_angle) * _scale;
	}

	_scale = 1.0;
	_scale_text->ChangeValue(""); 
	_angle = 0.0;
	_angle_text->ChangeValue("");
}

void p6::MoveBar::_refresh_construction() noexcept
{
	const std::set<uint> *selected_nodes = &_frame->main_panel()->selected_nodes;
	uint j = 0;
	for (auto i = selected_nodes->cbegin(); i != selected_nodes->cend(); i++, j++)
	{
		Coord relative_coord = _initial_node_coords[j] - _initial_anchor;
		_frame->construction()->set_node_coord(*i, _shift + _initial_anchor + relative_coord.rotate(_angle) * _scale);
	}
	
	const std::set<uint> *selected_forces = &_frame->main_panel()->selected_forces;
	j = 0;
	for (auto i = selected_forces->cbegin(); i != selected_forces->cend(); i++, j++)
	{
		_frame->construction()->set_force_direction(*i, _initial_force_directions[j].rotate(_angle) * _scale);
	}

	_frame->main_panel()->need_refresh();
}

p6::Coord p6::MoveBar::anchor() const noexcept
{
	return _initial_anchor + _shift;
}

void p6::MoveBar::set_anchor(wxPoint point) noexcept
{
	_refresh_scale_angle();
	_initial_anchor = _frame->main_panel()->pixel_to_real(point) - _shift;
	_frame->main_panel()->need_refresh();
}

void p6::MoveBar::drag_begin(wxPoint point) noexcept
{
	if (_allowed_shift->GetValue())
	{
		_old_shift = _shift;
		_drag_begin = _frame->main_panel()->pixel_to_real(point);
	}
	else if (_allowed_scale->GetValue())
	{
		_old_scale = _scale;
		_drag_begin = _frame->main_panel()->pixel_to_real(point);
	}
	else if (_allowed_rotate->GetValue())
	{
		_old_angle = _angle;
		_drag_begin = _frame->main_panel()->pixel_to_real(point);
	}
	else _frame->main_panel()->drag_begin(point);
}

void p6::MoveBar::drag_continue(wxPoint point) noexcept
{
	if (_allowed_shift->GetValue())
	{
		_shift = _old_shift + _frame->main_panel()->pixel_to_real(point) - _drag_begin;
		_shift_x_text->ChangeValue(real_to_string(_shift.x));
		_shift_y_text->ChangeValue(real_to_string(_shift.y));
		_anchor_x_text->ChangeValue(real_to_string(_shift.x + _initial_anchor.x));
		_anchor_y_text->ChangeValue(real_to_string(_shift.y + _initial_anchor.y));
		_refresh_construction();
	}
	else if (_allowed_scale->GetValue())
	{
		Coord coord = _frame->main_panel()->pixel_to_real(point);
		_scale = _old_scale * (coord - (_initial_anchor + _shift)).norm() / (_drag_begin - (_initial_anchor + _shift)).norm();
		_scale_text->ChangeValue(real_to_string(_scale));
		_refresh_construction();
	}
	else if (_allowed_rotate->GetValue())
	{
		Coord coord = _frame->main_panel()->pixel_to_real(point);
		_angle = _old_angle - (_drag_begin - (_initial_anchor + _shift)).angle() + (coord - (_initial_anchor + _shift)).angle();
		_angle_text->ChangeValue(real_to_string(180.0 * _angle / pi()));
		_refresh_construction();
	}
	else _frame->main_panel()->drag_continue(point);
}

p6::MoveBar::MoveBar(Frame *frame) noexcept : _frame(frame)
{
	wxWindow *parent = frame->side_panel()->panel();
	//Anchor x static
	_anchor_x_static = new wxStaticText(parent, wxID_ANY, "Anchor X:");
	_anchor_x_static->Show(false);
	//Anchor x text
	_anchor_x_text = new wxTextCtrl(parent, wxID_ANY);
	parent->Bind(wxEVT_TEXT, &MoveBar::_on_anchor_x, this, _anchor_x_text->GetId());
	_anchor_x_text->Show(false);
	//Anchor y static
	_anchor_y_static = new wxStaticText(parent, wxID_ANY, "Anchor Y:");
	_anchor_y_static->Show(false);
	//Anchor y text
	_anchor_y_text = new wxTextCtrl(parent, wxID_ANY);
	parent->Bind(wxEVT_TEXT, &MoveBar::_on_anchor_y, this, _anchor_y_text->GetId());
	_anchor_y_text->Show(false);
	//Scale static
	_scale_static = new wxStaticText(parent, wxID_ANY, "Scale:");
	_scale_static->Show(false);
	//Scale text
	_scale_text = new wxTextCtrl(parent, wxID_ANY);
	parent->Bind(wxEVT_TEXT, &MoveBar::_on_scale, this, _scale_text->GetId());
	_scale_text->Show(false);
	//Angle static
	_angle_static = new wxStaticText(parent, wxID_ANY, "Angle (grad):");
	_angle_static->Show(false);
	//Angle text
	_angle_text = new wxTextCtrl(parent, wxID_ANY);
	parent->Bind(wxEVT_TEXT, &MoveBar::_on_angle, this, _angle_text->GetId());
	_angle_text->Show(false);
	//Shift x static
	_shift_x_static = new wxStaticText(parent, wxID_ANY, "Shift X:");
	_shift_x_static->Show(false);
	//Shift x text
	_shift_x_text = new wxTextCtrl(parent, wxID_ANY);
	parent->Bind(wxEVT_TEXT, &MoveBar::_on_shift_x, this, _shift_x_text->GetId());
	_shift_x_text->Show(false);
	//Shift y static
	_shift_y_static = new wxStaticText(parent, wxID_ANY, "Shift Y:");
	_shift_y_static->Show(false);
	//Shift y text
	_shift_y_text = new wxTextCtrl(parent, wxID_ANY);
	parent->Bind(wxEVT_TEXT, &MoveBar::_on_shift_y, this, _shift_y_text->GetId());
	_shift_y_text->Show(false);
	//Allowed shift check box
	_allowed_shift = new wxCheckBox(parent, wxID_ANY, "Shift allowed");
	parent->Bind(wxEVT_CHECKBOX, &MoveBar::_on_allowed_shift, this, _allowed_shift->GetId());
	_allowed_shift->Show(false);
	//Allowed scale check box
	_allowed_scale = new wxCheckBox(parent, wxID_ANY, "Scale allowed");
	parent->Bind(wxEVT_CHECKBOX, &MoveBar::_on_allowed_scale, this, _allowed_scale->GetId());
	_allowed_scale->Show(false);
	//Allowed rotate check box
	_allowed_rotate = new wxCheckBox(parent, wxID_ANY, "Rotate allowed");
	parent->Bind(wxEVT_CHECKBOX, &MoveBar::_on_allowed_rotate, this, _allowed_rotate->GetId());
	_allowed_rotate->Show(false);
}

void p6::MoveBar::show() noexcept
{
	_initial_anchor.x = 0.0;
	_anchor_x_text->ChangeValue("");
	_initial_anchor.y = 0.0;
	_anchor_y_text->ChangeValue("");
	_scale = 1.0;
	_scale_text->ChangeValue("");
	_angle = 0.0;
	_angle_text->ChangeValue("");
	_shift.x = 0.0;
	_shift_x_text->ChangeValue("");
	_shift.y = 0.0;
	_shift_y_text->ChangeValue("");

	wxBoxSizer *sizer = _frame->side_panel()->sizer();
	sizer->Add(_anchor_x_static,	0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_anchor_x_text,		0, wxEXPAND | wxALL, 5);
	sizer->Add(_anchor_y_static,	0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_anchor_y_text,		0, wxEXPAND | wxALL, 5);
	sizer->Add(_scale_static,		0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_scale_text,			0, wxEXPAND | wxALL, 5);
	sizer->Add(_angle_static,		0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_angle_text,			0, wxEXPAND | wxALL, 5);
	sizer->Add(_shift_x_static,		0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_shift_x_text,		0, wxEXPAND | wxALL, 5);
	sizer->Add(_shift_y_static,		0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_shift_y_text,		0, wxEXPAND | wxALL, 5);
	sizer->Add(_allowed_shift,		0, wxEXPAND | wxALL, 5);
	sizer->Add(_allowed_scale,		0, wxEXPAND | wxALL, 5);
	sizer->Add(_allowed_rotate,		0, wxEXPAND | wxALL, 5);
	sizer->ShowItems(true);
	_frame->frame()->Layout();
	_frame->main_panel()->need_refresh();
}

void p6::MoveBar::refresh() noexcept
{
	const std::set<uint> *selected_nodes = &_frame->main_panel()->selected_nodes;
	_initial_node_coords.resize(selected_nodes->size());
	uint j = 0;
	for (auto i = selected_nodes->cbegin(); i != selected_nodes->cend(); i++, j++)
	{
		_initial_node_coords[j] = _frame->construction()->get_node_coord(*i);
	}

	const std::set<uint> *selected_forces = &_frame->main_panel()->selected_forces;
	_initial_force_directions.resize(selected_forces->size());
	j = 0;
	for (auto i = selected_forces->cbegin(); i != selected_forces->cend(); i++, j++)
	{
		_initial_force_directions[j] = _frame->construction()->get_force_direction(*i);
	}
}

void p6::MoveBar::hide() noexcept
{
	_frame->side_panel()->sizer()->ShowItems(false);
	_frame->side_panel()->sizer()->Clear();
	_frame->main_panel()->need_refresh();
}