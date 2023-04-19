/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_node_bar.hpp"
#include "../header/p6_frame.hpp"

void p6::NodeBar::_on_fixed(wxCommandEvent &e)
{
	if (!e.IsChecked()) _fixed_check->SetValue(true);
	else
	{
		_free1d_check->SetValue(false);
		_free2d_check->SetValue(false);
		std::set<uint> *selected_nodes = &_frame->main_panel()->selected_nodes;
		for (auto i = selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
			_frame->construction()->set_node_freedom(*i, 0);
		_frame->main_panel()->need_refresh();
	}
}

void p6::NodeBar::_on_free1d(wxCommandEvent &e)
{
	if (!e.IsChecked()) _free1d_check->SetValue(true);
	else
	{
		_fixed_check->SetValue(false);
		_free2d_check->SetValue(false);
		std::set<uint> *selected_nodes = &_frame->main_panel()->selected_nodes;
		for (auto i = selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
			_frame->construction()->set_node_freedom(*i, 1);
		_frame->main_panel()->need_refresh();
	}
}

void p6::NodeBar::_on_free2d(wxCommandEvent &e)
{
	if (!e.IsChecked()) _free2d_check->SetValue(true);
	else
	{
		_fixed_check->SetValue(false);
		_free1d_check->SetValue(false);
		std::set<uint> *selected_nodes = &_frame->main_panel()->selected_nodes;
		for (auto i = selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
			_frame->construction()->set_node_freedom(*i, 2);
		_frame->main_panel()->need_refresh();
	}
}

void p6::NodeBar::_on_x(wxCommandEvent &e)
{
	std::set<uint> *selected_nodes = &_frame->main_panel()->selected_nodes;
	real x = string_to_real(_x_text->GetValue().ToStdString());
	if (x == x)
	{
		for (auto i = selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
		{
			Coord coord = _frame->construction()->get_node_coord(*i);
			coord.x = x;
			_frame->construction()->set_node_coord(*i, coord);
		}
		_frame->main_panel()->need_refresh();
	}
}

void p6::NodeBar::_on_y(wxCommandEvent &e)
{
	std::set<uint> *selected_nodes = &_frame->main_panel()->selected_nodes;
	real y = string_to_real(_y_text->GetValue().ToStdString());
	if (y == y)
	{
		for (auto i = selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
		{
			Coord coord = _frame->construction()->get_node_coord(*i);
			coord.y = y;
			_frame->construction()->set_node_coord(*i, coord);
		}
		_frame->main_panel()->need_refresh();
	}
}

void p6::NodeBar::_on_x_vector(wxCommandEvent &e)
{
	std::set<uint> *selected_nodes = &_frame->main_panel()->selected_nodes;
	real x = string_to_real(_x_vector_text->GetValue().ToStdString());
	if (x == x)
	{
		for (auto i = selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
		{
			Coord vector = _frame->construction()->get_node_rail_vector(*i);
			vector.x = x;
			_frame->construction()->set_node_rail_vector(*i, vector);
		}
		_frame->main_panel()->need_refresh();
	}
}

void p6::NodeBar::_on_y_vector(wxCommandEvent &e)
{
	std::set<uint> *selected_nodes = &_frame->main_panel()->selected_nodes;
	real y = string_to_real(_y_vector_text->GetValue().ToStdString());
	if (y == y)
	{
		for (auto i = selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
		{
			Coord vector = _frame->construction()->get_node_rail_vector(*i);
			vector.y = y;
			_frame->construction()->set_node_rail_vector(*i, vector);
		}
		_frame->main_panel()->need_refresh();
	}
}

p6::NodeBar::NodeBar(Frame *frame) noexcept : _frame(frame)
{
	wxWindow *parent = _frame->side_panel()->panel();

	//Fixed check
	_fixed_check = new wxCheckBox(parent, wxID_ANY, "Fixed");
	_fixed_check->Show(false);
	parent->Bind(wxEVT_CHECKBOX, &NodeBar::_on_fixed, this, _fixed_check->GetId());
	//Fixed on rail check
	_free1d_check = new wxCheckBox(parent, wxID_ANY, "Fixed on rail");
	_free1d_check->Show(false);
	parent->Bind(wxEVT_CHECKBOX, &NodeBar::_on_free1d, this, _free1d_check->GetId());
	//Free check
	_free2d_check = new wxCheckBox(parent, wxID_ANY, "Free");
	_free2d_check->Show(false);
	parent->Bind(wxEVT_CHECKBOX, &NodeBar::_on_free2d, this, _free2d_check->GetId());
	//X static text
	_x_static = new wxStaticText(parent, wxID_ANY, "X:");
	_x_static->Show(false);
	//X edit
	_x_text = new wxTextCtrl(parent, wxID_ANY);
	_x_text->Show(false);
	parent->Bind(wxEVT_TEXT, &NodeBar::_on_x, this, _x_text->GetId());
	//Y static text
	_y_static = new wxStaticText(parent, wxID_ANY, "Y:");
	_y_static->Show(false);
	//Y edit
	_y_text = new wxTextCtrl(parent, wxID_ANY);
	_y_text->Show(false);
	parent->Bind(wxEVT_TEXT, &NodeBar::_on_y, this, _y_text->GetId());
	//Rail X static text
	_x_vector_static = new wxStaticText(parent, wxID_ANY, "Rail vector X (fixed on rail only):");
	_x_vector_static->Show(false);
	//Rail X edit
	_x_vector_text = new wxTextCtrl(parent, wxID_ANY);
	_x_vector_text->Show(false);
	parent->Bind(wxEVT_TEXT, &NodeBar::_on_x_vector, this, _x_vector_text->GetId());
	//Rail Y static text
	_y_vector_static = new wxStaticText(parent, wxID_ANY, "Rail vector Y (fixed on rail only):");
	_y_vector_static->Show(false);
	//Rail Y edit
	_y_vector_text = new wxTextCtrl(parent, wxID_ANY);
	_y_vector_text->Show(false);
	parent->Bind(wxEVT_TEXT, &NodeBar::_on_y_vector, this, _y_vector_text->GetId());
}

void p6::NodeBar::show() noexcept
{
	wxBoxSizer *sizer = _frame->side_panel()->sizer();
	sizer->Add(_fixed_check,	0, wxEXPAND | wxALL, 5);
	sizer->Add(_free1d_check,	0, wxEXPAND | wxALL, 5);
	sizer->Add(_free2d_check,	0, wxEXPAND | wxALL, 5);
	sizer->Add(_x_static,		0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_x_text,			0, wxEXPAND | wxALL, 5);
	sizer->Add(_y_static,		0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_y_text,			0, wxEXPAND | wxALL, 5);
	sizer->Add(_x_vector_static,0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_x_vector_text,	0, wxEXPAND | wxALL, 5);
	sizer->Add(_y_vector_static,0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_y_vector_text,	0, wxEXPAND | wxALL, 5);
	sizer->ShowItems(true);
	_frame->frame()->Layout();
}

void p6::NodeBar::refresh() noexcept
{
	std::set<uint> *selected_nodes = &_frame->main_panel()->selected_nodes;
	if (selected_nodes->empty()) return;
	Construction *con = _frame->construction();
	bool sim = _frame->toolbar()->simulation();

	//Setting freedom degree
	{
		bool has_fixed = false;
		bool has_free1d = false;
		bool has_free2d = false;
		for (auto i = selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
		{
			if (con->get_node_freedom(*i) == 0) has_fixed = true;
			else if (con->get_node_freedom(*i) == 1) has_free1d = true;
			else has_free2d = true;
			if (has_fixed && has_free1d && has_free2d) break;
		}
		_fixed_check->Enable(!sim);
		_fixed_check->SetValue(has_fixed);
		_free1d_check->Enable(!sim);
		_free1d_check->SetValue(has_free1d);
		_free2d_check->Enable(!sim);
		_free2d_check->SetValue(has_free2d);
	}

	//Setting x
	{
		bool x_equal = true;
		real x_value = con->get_node_coord(*selected_nodes->cbegin()).x;
		for (auto i = ++selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
		{
			if (x_value != con->get_node_coord(*i).x) { x_equal = false; break; }
		}
		if (x_equal) _x_text->ChangeValue(real_to_string(x_value));
		else _x_text->ChangeValue("");
		_x_text->Enable(!sim);
	}

	//Setting y
	{
		bool y_equal = true;
		real y_value = con->get_node_coord(*selected_nodes->cbegin()).y;
		for (auto i = ++selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
		{
			if (y_value != con->get_node_coord(*i).y) { y_equal = false; break; }
		}
		if (y_equal) _y_text->ChangeValue(real_to_string(y_value));
		else _y_text->ChangeValue("");
		_y_text->Enable(!sim);
	}

	//Setting X vector
	{
		bool x_equal = true;
		real x_value = con->get_node_rail_vector(*selected_nodes->cbegin()).x;
		for (auto i = ++selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
		{
			if (x_value != con->get_node_rail_vector(*i).x) { x_equal = false; break; }
		}
		if (x_equal) _x_vector_text->ChangeValue(real_to_string(x_value));
		else _x_vector_text->ChangeValue("");
		_x_vector_text->Enable(!sim);
	}

	//Setting Y vector
	{
		bool y_equal = true;
		real y_value = con->get_node_rail_vector(*selected_nodes->cbegin()).y;
		for (auto i = ++selected_nodes->cbegin(); i != selected_nodes->cend(); i++)
		{
			if (y_value != con->get_node_rail_vector(*i).y) { y_equal = false; break; }
		}
		if (y_equal) _y_vector_text->ChangeValue(real_to_string(y_value));
		else _y_vector_text->ChangeValue("");
		_y_vector_text->Enable(!sim);
	}
}

void p6::NodeBar::hide() noexcept
{
	_frame->side_panel()->sizer()->ShowItems(false);
	_frame->side_panel()->sizer()->Clear();
}