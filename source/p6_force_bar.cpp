/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_force_bar.hpp"
#include "../header/p6_frame.hpp"
#include <cassert>

void p6::ForceBar::_on_x(wxCommandEvent &e)
{
	real x = string_to_real(_x_text->GetValue().ToStdString());
	if (x == x)
	{
		std::set<uint> *selected_forces = &_frame->main_panel()->selected_forces;
		for (auto i = selected_forces->cbegin(); i != selected_forces->cend(); i++)
		{
			Coord direction = _frame->construction()->get_force_direction(*i);
			direction.x = x;
			_modulus_text->ChangeValue(real_to_string(direction.norm()));
			_angle_text->ChangeValue(real_to_string(180.0 * direction.angle() / pi()));
			_frame->construction()->set_force_direction(*i, direction);
			_frame->main_panel()->need_refresh();
		}
	}
}

void p6::ForceBar::_on_y(wxCommandEvent &e)
{
	real y = string_to_real(_y_text->GetValue().ToStdString());
	if (y == y)
	{
		std::set<uint> *selected_forces = &_frame->main_panel()->selected_forces;
		for (auto i = selected_forces->cbegin(); i != selected_forces->cend(); i++) {
			Coord direction = _frame->construction()->get_force_direction(*i);
			direction.y = y;
			_modulus_text->ChangeValue(real_to_string(direction.norm()));
			_angle_text->ChangeValue(real_to_string(180.0 * direction.angle() / pi()));
			_frame->construction()->set_force_direction(*i, direction);
			_frame->main_panel()->need_refresh();
		}
	}
}

void p6::ForceBar::_on_modulus(wxCommandEvent &e)
{
	real m = string_to_real(_modulus_text->GetValue().ToStdString());
	if (m == m)
	{
		std::set<uint> *selected_forces = &_frame->main_panel()->selected_forces;
		for (auto i = selected_forces->cbegin(); i != selected_forces->cend(); i++)
		{
			real angle = _frame->construction()->get_force_direction(*i).angle();
			Coord direction = Coord(cos(angle), sin(angle)) * m;
			_x_text->ChangeValue(real_to_string(direction.x));
			_y_text->ChangeValue(real_to_string(direction.y));
			_frame->construction()->set_force_direction(*i, direction);
			_frame->main_panel()->need_refresh();
		}
	}
}

void p6::ForceBar::_on_angle(wxCommandEvent &e)
{
	real a = pi() * string_to_real(_angle_text->GetValue().ToStdString()) / 180.0;
	if (a == a)
	{
		std::set<uint> *selected_forces = &_frame->main_panel()->selected_forces;
		for (auto i = selected_forces->cbegin(); i != selected_forces->cend(); i++)
		{
			real modulus = _frame->construction()->get_force_direction(*i).norm();
			Coord direction = Coord(cos(a), sin(a)) * modulus;
			_x_text->ChangeValue(real_to_string(direction.x));
			_y_text->ChangeValue(real_to_string(direction.y));
			_frame->construction()->set_force_direction(*i, direction);
			_frame->main_panel()->need_refresh();
		}
	}
}

p6::ForceBar::ForceBar(Frame *frame)  noexcept : _frame(frame)
{
	wxWindow *parent = frame->side_panel()->panel();
	
	//Static x
	_x_static = new wxStaticText(parent, wxID_ANY, "X:");
	_x_static->Show(false);
	//Edit x
	_x_text = new wxTextCtrl(parent, wxID_ANY);
	parent->Bind(wxEVT_TEXT, &ForceBar::_on_x, this, _x_text->GetId());
	_x_text->Show(false);
	//Static x
	_y_static = new wxStaticText(parent, wxID_ANY, "Y:");
	_y_static->Show(false);
	//Edit y
	_y_text = new wxTextCtrl(parent, wxID_ANY);
	parent->Bind(wxEVT_TEXT, &ForceBar::_on_y, this, _y_text->GetId());
	_y_text->Show(false);
	//Static modulus
	_modulus_static = new wxStaticText(parent, wxID_ANY, "Modulus:");
	_modulus_static->Show(false);
	//Edit modulus
	_modulus_text = new wxTextCtrl(parent, wxID_ANY);
	parent->Bind(wxEVT_TEXT, &ForceBar::_on_modulus, this, _modulus_text->GetId());
	_modulus_text->Show(false);
	//Static angle
	_angle_static = new wxStaticText(parent, wxID_ANY, "Angle (grad):");
	_angle_static->Show(false);
	//Edit angle
	_angle_text = new wxTextCtrl(parent, wxID_ANY);
	parent->Bind(wxEVT_TEXT, &ForceBar::_on_angle, this, _angle_text->GetId());
	_angle_text->Show(false);
}

void p6::ForceBar::show() noexcept
{
	wxBoxSizer *sizer = _frame->side_panel()->sizer();
	sizer->Add(_x_static,		0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_x_text,			0, wxEXPAND | wxALL, 5);
	sizer->Add(_y_static,		0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_y_text,			0, wxEXPAND | wxALL, 5);
	sizer->Add(_modulus_static,	0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_modulus_text,	0, wxEXPAND | wxALL, 5);
	sizer->Add(_angle_static,	0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_angle_text,		0, wxEXPAND | wxALL, 5);
	sizer->ShowItems(true);
	_frame->frame()->Layout();
}

void p6::ForceBar::refresh() noexcept
{
	bool sim = _frame->toolbar()->simulation();
	std::set<uint> *selected_forces = &_frame->main_panel()->selected_forces;

	//Set x
	{
		bool x_equal = true;
		real x_value = _frame->construction()->get_force_direction(*selected_forces->cbegin()).x;
		for (auto i = ++selected_forces->cbegin(); i != selected_forces->cend(); i++)
		{
			if (x_value != _frame->construction()->get_force_direction(*i).x) { x_equal = false; break; }
		}
		if (x_equal) _x_text->ChangeValue(real_to_string(x_value));
		else _x_text->ChangeValue("");
		_x_text->Enable(!sim);
	}

	//Set y
	{
		bool y_equal = true;
		real y_value = _frame->construction()->get_force_direction(*selected_forces->cbegin()).y;
		for (auto i = ++selected_forces->cbegin(); i != selected_forces->cend(); i++)
		{
			if (y_value != _frame->construction()->get_force_direction(*i).y) { y_equal = false; break; }
		}
		if (y_equal) _y_text->ChangeValue(real_to_string(y_value));
		else _y_text->ChangeValue("");
		_y_text->Enable(!sim);
	}

	//Set modulus
	{
		bool modulus_equal = true;
		real modulus_value = _frame->construction()->get_force_direction(*selected_forces->cbegin()).norm();
		for (auto i = ++selected_forces->cbegin(); i != selected_forces->cend(); i++)
		{
			if (modulus_value != _frame->construction()->get_force_direction(*i).norm()) { modulus_equal = false; break; }
		}
		if (modulus_equal) _modulus_text->ChangeValue(real_to_string(modulus_value));
		else _modulus_text->ChangeValue("");
		_modulus_text->Enable(!sim);
	}

	//Set angle
	{
		bool angle_equal = true;
		real angle_value = _frame->construction()->get_force_direction(*selected_forces->cbegin()).angle();
		for (auto i = ++selected_forces->cbegin(); i != selected_forces->cend(); i++)
		{
			if (angle_value != _frame->construction()->get_force_direction(*i).angle()) { angle_equal = false; break; }
		}
		if (angle_equal) _angle_text->ChangeValue(real_to_string(180.0 * angle_value / pi()));
		else _angle_text->ChangeValue("");
		_angle_text->Enable(!sim);
	}
}

void p6::ForceBar::hide() noexcept
{
	_frame->side_panel()->sizer()->ShowItems(false);
	_frame->side_panel()->sizer()->Clear();
}