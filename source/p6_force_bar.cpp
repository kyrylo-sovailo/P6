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

void p6::ForceBar::_on_force_x(wxCommandEvent &e)
{
	real x = string_to_real(_x_text->GetValue().ToStdString());
	if (x == x)
	{
		std::set<uint> *selected_forces = &_frame->main_panel()->selected_forces;
		for (auto i = selected_forces->cbegin(); i != selected_forces->cend(); i++)
		{
			Coord direction = _frame->construction()->get_force_direction(*i);
			direction.x = x;
			_frame->construction()->set_force_direction(*i, direction);
			_frame->main_panel()->need_refresh();
		}
	}
}

void p6::ForceBar::_on_force_y(wxCommandEvent &e)
{
	real y = string_to_real(_y_text->GetValue().ToStdString());
	if (y == y)
	{
		std::set<uint> *selected_forces = &_frame->main_panel()->selected_forces;
		for (auto i = selected_forces->cbegin(); i != selected_forces->cend(); i++) {
			Coord direction = _frame->construction()->get_force_direction(*i);
			direction.y = y;
			_frame->construction()->set_force_direction(*i, direction);
			_frame->main_panel()->need_refresh();
		}
	}
}

p6::ForceBar::ForceBar(Frame *frame) : _frame(frame)
{
	wxWindow *parent = frame->side_panel()->panel();
	
	//Static x
	_x_static = new wxStaticText(parent, wxID_ANY, "X:");
	_x_static->Show(false);
	
	//Edit x
	_x_text = new wxTextCtrl(parent, wxID_ANY);
	parent->Bind(wxEVT_TEXT, &ForceBar::_on_force_x, this, _x_text->GetId());
	_x_text->Show(false);
	
	//Static x
	_y_static = new wxStaticText(parent, wxID_ANY, "Y:");
	_y_static->Show(false);
	
	//Edit y
	_y_text = new wxTextCtrl(parent, wxID_ANY);
	parent->Bind(wxEVT_TEXT, &ForceBar::_on_force_y, this, _y_text->GetId());
	_y_text->Show(false);
}

void p6::ForceBar::show()
{
	wxBoxSizer *sizer = _frame->side_panel()->sizer();
	sizer->Add(_x_static,	0, wxEXPAND | wxALL, 10);
	sizer->Add(_x_text,		0, wxEXPAND | wxALL, 10);
	sizer->Add(_y_static,	0, wxEXPAND | wxALL, 10);
	sizer->Add(_y_text,		0, wxEXPAND | wxALL, 10);
	sizer->ShowItems(true);
	_frame->side_panel()->panel()->Layout();
}

void p6::ForceBar::refresh()
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
}

void p6::ForceBar::hide()
{
	_frame->side_panel()->sizer()->ShowItems(false);
	_frame->side_panel()->sizer()->Clear();
}