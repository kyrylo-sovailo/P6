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

void p6::StickBar::_on_area(wxCommandEvent &e)
{
	std::set<uint> *selected_sticks = &_frame->main_panel()->selected_sticks;
	real a = string_to_real(_area_text->GetValue().ToStdString());
	if (a == a && a > 0.0)
	{
		for (auto i = selected_sticks->cbegin(); i != selected_sticks->cend(); i++)
			_frame->construction()->set_stick_area(*i, a);
	}
}

void p6::StickBar::_on_material(wxCommandEvent &e)
{
	std::set<uint> *selected_sticks = &_frame->main_panel()->selected_sticks;
	int c = _material_choice->GetSelection();
	if (c != wxNOT_FOUND)
	{
		for (auto i = selected_sticks->cbegin(); i != selected_sticks->cend(); i++)
			_frame->construction()->set_stick_material(*i, c);
	}
}

p6::StickBar::StickBar(Frame *frame)  noexcept : _frame(frame)
{
	wxWindow *parent = frame->side_panel()->panel();

	//Material static text
	_material_static = new wxStaticText(parent, wxID_ANY, "Material:");
	_material_static->Show(false);
	//Material choice
	_material_choice = new wxChoice(parent, wxID_ANY);
	_material_choice->Show(false);
	parent->Bind(wxEVT_CHOICE, &StickBar::_on_material, this, _material_choice->GetId());
	//Area static text
	_area_static = new wxStaticText(parent, wxID_ANY, "Area:");
	_area_static->Show(false);
	//Area text
	_area_text = new wxTextCtrl(parent, wxID_ANY);
	_area_text->Show(false);
	parent->Bind(wxEVT_TEXT, &StickBar::_on_area, this, _area_text->GetId());
	//Length static text
	_length_static = new wxStaticText(parent, wxID_ANY, "Length:");
	_length_static->Show(false);
	//Length text
	_length_text = new wxTextCtrl(parent, wxID_ANY);
	_length_text->Enable(false);
	_length_text->Show(false);
	//Strain static text
	_strain_static = new wxStaticText(parent, wxID_ANY, "Strain:");
	_strain_static->Show(false);
	//Strain text
	_strain_text = new wxTextCtrl(parent, wxID_ANY);
	_strain_text->Enable(false);
	_strain_text->Show(false);
	//Force static text
	_force_static = new wxStaticText(parent, wxID_ANY, "Force:");
	_force_static->Show(false);
	//Force text
	_force_text = new wxTextCtrl(parent, wxID_ANY);
	_force_text->Enable(false);
	_force_text->Show(false);
}

void p6::StickBar::show() noexcept
{
	wxBoxSizer *sizer = _frame->side_panel()->sizer();
	sizer->Add(_material_static,	0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_material_choice,	0, wxEXPAND | wxALL, 5);
	sizer->Add(_area_static,		0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_area_text,			0, wxEXPAND | wxALL, 5);
	sizer->Add(_length_static,		0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_length_text,		0, wxEXPAND | wxALL, 5);
	sizer->Show(true);
	sizer->Add(_strain_static,		0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_strain_text,		0, wxEXPAND | wxALL, 5);
	sizer->Add(_force_static,		0, wxEXPAND | wxRIGHT | wxLEFT | wxTOP, 5);
	sizer->Add(_force_text,			0, wxEXPAND | wxALL, 5);
	if (_frame->toolbar()->simulation()) sizer->ShowItems(true);
	//P6 refreshes bars before showing
	//So even if I hide panels during refresh, they will be shown here
	//That's why I need some extra code
	//But with Enable() works fine, it isn't changed when Show()
	_frame->frame()->Layout();
}

void p6::StickBar::refresh() noexcept
{
	std::set<uint> *selected_sticks = &_frame->main_panel()->selected_sticks;
	bool sim = _frame->toolbar()->simulation();
	Construction *con = _frame->construction();

	//Setting material
	{
		bool material_equal = true;
		uint material_value = con->get_stick_material(*selected_sticks->cbegin());
		for (auto i = ++selected_sticks->cbegin(); i != selected_sticks->cend(); i++)
		{
			if (material_value != con->get_stick_material(*i)) { material_equal = false; break; }
		}
		if (!material_equal) _material_choice->SetSelection(wxNOT_FOUND);
		else _material_choice->SetSelection(material_value);
		_material_choice->Enable(!sim);
	}

	//Setting area
	{
		bool area_equal = true;
		real area_value = con->get_stick_area(*selected_sticks->cbegin());
		for (auto i = ++selected_sticks->cbegin(); i != selected_sticks->cend(); i++)
		{
			if (area_value != con->get_stick_area(*i)) { area_equal = false; break; }
		}
		if (area_equal) _area_text->ChangeValue(real_to_string(area_value));
		else _area_text->ChangeValue("");
		_area_text->Enable(!sim);
	}

	//Setting length
	{
		bool length_equal = true;
		real length_value = con->get_stick_length(*selected_sticks->cbegin());
		for (auto i = ++selected_sticks->cbegin(); i != selected_sticks->cend(); i++)
		{
			if (length_value != con->get_stick_length(*i)) { length_equal = false; break; }
		}
		if (length_equal) _length_text->ChangeValue(real_to_string(length_value));
		else _length_text->ChangeValue("");
	}

	_strain_static->Show(sim);
	_strain_text->Show(sim);
	_force_static->Show(sim);
	_force_text->Show(sim);
	if (sim)
	{
		//Setting strain
		{
			bool strain_equal = true;
			real strain_value = con->get_stick_strain(*selected_sticks->cbegin());
			for (auto i = ++selected_sticks->cbegin(); i != selected_sticks->cend(); i++)
			{
				if (strain_value != con->get_stick_strain(*i)) { strain_equal = false; break; }
			}
			if (!strain_equal) _strain_text->ChangeValue(real_to_string(strain_value));
			else _strain_text->ChangeValue("");
		}
		
		//Setting force
		{
			bool force_equal = true;
			real force_value = con->get_stick_force(*selected_sticks->cbegin());
			for (auto i = ++selected_sticks->cbegin(); i != selected_sticks->cend(); i++)
			{
				if (force_value != con->get_stick_force(*i)) { force_equal = false; break; }
			}
			if (!force_equal) _force_text->ChangeValue(real_to_string(force_value));
			else _force_text->ChangeValue("");
		}
	}
}

void p6::StickBar::refresh_materials() noexcept
{
	wxWindow *parent = _frame->side_panel()->panel();

	//Unbind old
	int choice = _material_choice->GetSelection();
	bool shown = _material_choice->IsShown();
	bool enabled = _material_choice->IsEnabled();
	parent->Unbind(wxEVT_CHOICE, &StickBar::_on_material, this, _material_choice->GetId());

	//Create new
	wxArrayString array;
	array.Alloc(_frame->construction()->get_material_count());
	for (uint i = 0; i < _frame->construction()->get_material_count(); i++)
		array.Add(_frame->construction()->get_material_name(i));
	wxChoice *newchoice = new wxChoice(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, array);

	//Replace
	_frame->side_panel()->sizer()->Replace(_material_choice, newchoice);
	_material_choice->Destroy();
	_material_choice = newchoice;

	//Bind new
	_material_choice->SetSelection(choice);
	_material_choice->Show(shown);
	_material_choice->Enable(enabled);
	parent->Bind(wxEVT_CHOICE, &StickBar::_on_material, this, _material_choice->GetId());
}

void p6::StickBar::hide() noexcept
{
	_frame->side_panel()->sizer()->ShowItems(false);
	_frame->side_panel()->sizer()->Clear();
}