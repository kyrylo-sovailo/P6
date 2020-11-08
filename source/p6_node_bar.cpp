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
#include <set>

void p6::NodeBar::_on_free(wxCommandEvent &e)
{
	std::set<uint> *selected_nodes = &_frame->main_panel.selected_nodes;
	_fixed_check->SetValue(!_free_check->GetValue());
	for (auto i = selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
		_frame->construction.set_node_free(*i, _free_check->GetValue());
}

void p6::NodeBar::_on_fixed(wxCommandEvent &e)
{
	std::set<uint> *selected_nodes = &_frame->main_panel.selected_nodes;
	_free_check->SetValue(!_fixed_check->GetValue());
	for (auto i = selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
		_frame->construction.set_node_free(*i, _free_check->GetValue());
}

void p6::NodeBar::_on_x(wxCommandEvent &e)
{
	std::set<uint> *selected_nodes = &_frame->main_panel.selected_nodes;
	real x = Utils::string_to_real(_x_text->GetValue().ToStdString());
	if (x == x)
	{
		for (auto i = selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
			_frame->construction.set_node_x(*i, x);
	}
}

void p6::NodeBar::_on_y(wxCommandEvent &e)
{
	std::set<uint> *selected_nodes = &_frame->main_panel.selected_nodes;
	real y = Utils::string_to_real(_y_text->GetValue().ToStdString());
	if (y == y)
	{
		for (auto i = selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
			_frame->construction.set_node_y(*i, y);
	}
}

p6::NodeBar::NodeBar(Frame *frame)
{
	_frame = frame;
	wxWindow *parent = _frame->side_panel.panel;

	//Free check
	_free_check = new wxCheckBox(parent, wxID_ANY, "Free");
	_free_check->Show(false);
	parent->Bind(wxEVT_CHECKBOX, &NodeBar::_on_free, this, _free_check->GetId());
	//Fixed check
	_fixed_check = new wxCheckBox(parent, wxID_ANY, "Fixed");
	_fixed_check->Show(false);
	parent->Bind(wxEVT_CHECKBOX, &NodeBar::_on_fixed, this, _fixed_check->GetId());
	//X static text
	_x_static = new wxStaticText(parent, wxID_ANY, "X:");
	_x_static->Show(false);
	//X edit
	_x_text = new wxTextCtrl(parent, wxID_ANY);
	_x_text->Show(false);
	parent->Bind(wxEVT_TEXT, &NodeBar::_on_x, this, _x_text->GetId());
	//Y static text
	_y_static = new wxStaticText(parent, wxID_ANY, "X:");
	_y_static->Show(false);
	//Y edit
	_y_text = new wxTextCtrl(parent, wxID_ANY);
	_y_text->Show(false);
	parent->Bind(wxEVT_TEXT, &NodeBar::_on_y, this, _y_text->GetId());
}

void p6::NodeBar::show()
{
	wxBoxSizer *sizer = _frame->side_panel.sizer;
	sizer->Add(_free_check, 0, wxALL | wxEXPAND, 10);
	sizer->Add(_fixed_check, 0, wxALL | wxEXPAND, 10);
	sizer->Add(_x_static, 0, wxALL | wxEXPAND, 10);
	sizer->Add(_x_text, 0, wxALL | wxEXPAND, 10);
	sizer->Add(_y_static, 0, wxALL | wxEXPAND, 10);
	sizer->Add(_y_text, 0, wxALL | wxEXPAND, 10);
}

void p6::NodeBar::refresh()
{
	Construction *con = &_frame->construction;
	std::set<uint> *selected_nodes = &_frame->main_panel.selected_nodes;

	//Setting free
	{
		bool free_equal = true;
		bool free_value = con->get_node_free(*selected_nodes->cbegin());
		for (auto i = ++selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
		{
			if (free_value != con->get_node_free(*i)) { free_equal = false; break; }
		}
		_free_check->SetValue(!free_equal || free_value);
		_fixed_check->SetValue(!free_equal || !free_value);
	}

	//Setting x
	{
		bool x_equal = true;
		real x_value = con->get_node_x(*selected_nodes->cbegin());
		for (auto i = ++selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
		{
			if (x_value != con->get_node_x(*i)) { x_equal = false; break; }
		}
		if (x_equal) _x_text->ChangeValue(Utils::real_to_string(x_value));
		else _x_text->ChangeValue("");
	}

	//Setting y
	{
		bool y_equal = true;
		real y_value = con->get_node_y(*selected_nodes->cbegin());
		for (auto i = ++selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
		{
			if (y_value != con->get_node_y(*i)) { y_equal = false; break; }
		}
		if (y_equal) _y_text->ChangeValue(Utils::real_to_string(y_value));
		else _y_text->ChangeValue("");
	}
}

void p6::NodeBar::hide()
{}