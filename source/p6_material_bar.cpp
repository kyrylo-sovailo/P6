/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_material_bar.hpp"
#include "../header/p6_frame.hpp"
#include "../header/p6_linear_material.hpp"
#include "../header/p6_nonlinear_material.hpp"

void p6::MaterialBar::_on_choice(wxCommandEvent &e)
{
	int c = _material_choice->GetSelection();
	if (c == wxNOT_FOUND)
	{
		_name_text->ChangeValue("");
		_nonlinear_check->SetValue(false);
		_formula_text->ChangeValue("");
	}
	else
	{
		Construction *con = _frame->construction();
		_name_text->ChangeValue(con->get_material_name(c));
		bool linear = (con->get_material_type(c) == Material::Type::linear);
		_nonlinear_check->SetValue(!linear);
		_formula_text->ChangeValue(linear ?
			real_to_string(con->get_material_modulus(c)) :
			con->get_material_formula(c));
	}
}

void p6::MaterialBar::_on_new(wxCommandEvent &e)
{
	_material_choice->SetSelection(wxNOT_FOUND);
	_name_text->ChangeValue("");
	_nonlinear_check->SetValue(false);
	_formula_text->ChangeValue("");
}

void p6::MaterialBar::_on_apply(wxCommandEvent &e)
{
	try
	{
		Construction *con = _frame->construction();
		String name = _name_text->GetValue().ToStdString();
		String formula = _formula_text->GetValue().ToStdString();
		uint material = _nonlinear_check->GetValue() ?
			con->create_nonlinear_material(name, formula) :
			con->create_linear_material(name, string_to_real(formula));
		_frame->side_panel()->refresh_materials();
		_material_choice->SetSelection(material);
	}
	catch (std::exception &e)
	{
		wxMessageBox(e.what(), "Error", wxICON_ERROR, _frame->frame());
	}
}

void p6::MaterialBar::_on_delete(wxCommandEvent &e)
{
	int c = _material_choice->GetSelection();
	if (c != wxNOT_FOUND)
	{
		Construction *con = _frame->construction();
		for (uint i = 0; i < con->get_stick_count(); i++)
		{
			if (con->get_stick_material(i) == c) con->set_stick_material(i, c);
		}
		con->delete_material(c);
		_frame->side_panel()->refresh_materials();
	}
}

p6::MaterialBar::MaterialBar(Frame *frame)  noexcept : _frame(frame)
{
	wxWindow *parent = frame->side_panel()->panel();

	//Material choice
	_material_choice = new wxChoice(parent, wxID_ANY);
	parent->Bind(wxEVT_CHOICE, &MaterialBar::_on_choice, this, _material_choice->GetId());
	//Name static text
	_name_static = new wxStaticText(parent, wxID_ANY, "Name:");
	//Name edit
	_name_text = new wxTextCtrl(parent, wxID_ANY);
	//Nonlinear check
	_nonlinear_check = new wxCheckBox(parent, wxID_ANY, "Non-linear");
	//Formula static text
	_formula_static = new wxStaticText(parent, wxID_ANY, "Modulus / formula:");
	//Formula edit
	_formula_text = new wxTextCtrl(parent, wxID_ANY);
	//New-accept-delete buttons
	_buttons_sizer = new wxBoxSizer(wxHORIZONTAL);
	_button_new = new wxButton(parent, wxID_ANY, "New");
	_buttons_sizer->Add(_button_new, 1, wxALL, 10);
	parent->Bind(wxEVT_BUTTON, &MaterialBar::_on_new, this, _button_new->GetId());
	_button_apply = new wxButton(parent, wxID_ANY, "Apply");
	_buttons_sizer->Add(_button_apply, 1, wxALL, 10);
	parent->Bind(wxEVT_BUTTON, &MaterialBar::_on_apply, this, _button_apply->GetId());
	_button_delete = new wxButton(parent, wxID_ANY, "Delete");
	_buttons_sizer->Add(_button_delete, 1, wxALL, 10);
	parent->Bind(wxEVT_BUTTON, &MaterialBar::_on_delete, this, _button_delete->GetId());
}

void p6::MaterialBar::show() noexcept
{
	int c = _material_choice->GetSelection();
	if (c == wxNOT_FOUND)
	{
		_name_text->ChangeValue("");
		_nonlinear_check->SetValue(false);
		_formula_text->ChangeValue("");
	}
	else
	{
		Construction *con = _frame->construction();
		_name_text->ChangeValue(con->get_material_name(c));
		bool nonlinear = con->get_material_type(c) == Material::Type::nonlinear;
		_nonlinear_check->SetValue(nonlinear);
		_formula_text->ChangeValue(nonlinear ?
			con->get_material_formula(c) :
			real_to_string(con->get_material_modulus(c)));
	}

	wxBoxSizer *sizer = _frame->side_panel()->sizer();
	sizer->Add(_material_choice,	0, wxALL | wxEXPAND, 10);
	sizer->Add(_name_static,		0, wxALL | wxEXPAND, 10);
	sizer->Add(_name_text,			0, wxALL | wxEXPAND, 10);
	sizer->Add(_nonlinear_check,	0, wxALL | wxEXPAND, 10);
	sizer->Add(_formula_static,		0, wxALL | wxEXPAND, 10);
	sizer->Add(_formula_text,		0, wxALL | wxEXPAND, 10);
	sizer->Add(_buttons_sizer,		0, wxALL | wxEXPAND, 10);
	sizer->ShowItems(true);
	_frame->side_panel()->panel()->Layout();
}

void p6::MaterialBar::refresh_controls() noexcept
{
	bool sim = _frame->toolbar()->simulation();
	_name_text->Enable(!sim);
	_nonlinear_check->Enable(!sim);
	_formula_text->Enable(!sim);
	_button_new->Enable(!sim);
	_button_apply->Enable(!sim);
	_button_delete->Enable(!sim);
}

void p6::MaterialBar::refresh_materials() noexcept
{
	wxWindow *parent = _frame->side_panel()->panel();

	//Unbind old
	int choice = _material_choice->GetSelection();
	bool shown = _material_choice->IsShown();
	parent->Unbind(wxEVT_CHOICE, &MaterialBar::_on_choice, this, _material_choice->GetId());
	
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
	parent->Bind(wxEVT_CHOICE, &MaterialBar::_on_choice, this, _material_choice->GetId());
}

void p6::MaterialBar::hide() noexcept
{
	_frame->side_panel()->sizer()->ShowItems(false);
	_frame->side_panel()->sizer()->Detach(_buttons_sizer);
	_frame->side_panel()->sizer()->Clear();
}