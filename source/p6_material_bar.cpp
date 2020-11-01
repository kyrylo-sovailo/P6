#include "../header/p6_material_bar.h"

p6::MaterialBar::MaterialBar()
{
	//Material choice
	_name_choice = new wxChoice(_side_panel, wxID_ANY);
	Bind(wxEVT_CHOICE, &Frame::OnMaterialChoice, this, _name_choice->GetId());
	//Name edit
	_sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Name:"), 0, wxLEFT | wxEXPAND, 10);
	_name_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_sizer->Add(_name_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnMaterialName, this, _name_edit->GetId());
	//Nonlinear check
	_nonlinear_check = new wxCheckBox(_side_panel, wxID_ANY, "Non-linear");
	_sizer->Add(_nonlinear_check, 0, wxALL, 10);
	Bind(wxEVT_CHECKBOX, &Frame::OnMaterialNonlinear, this, _nonlinear_check->GetId());
	//Formula edit
	_sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Modulus / formula:"), 0, wxLEFT | wxEXPAND, 10);
	_formula_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_sizer->Add(_formula_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnMaterialFormula, this, _formula_edit->GetId());
	//New-accept-delete buttons
	wxBoxSizer *hsizer = new wxBoxSizer(wxHORIZONTAL);
	_new_button = new wxButton(_side_panel, wxID_ANY, "New");
	hsizer->Add(_new_button, 1, wxALL, 10);
	Bind(wxEVT_BUTTON, &Frame::OnMaterialNew, this, _new_button->GetId());
	_apply_button = new wxButton(_side_panel, wxID_ANY, "Apply");
	hsizer->Add(_apply_button, 1, wxALL, 10);
	Bind(wxEVT_BUTTON, &Frame::OnMaterialApply, this, _apply_button->GetId());
	_delete_button = new wxButton(_side_panel, wxID_ANY, "Delete");
	hsizer->Add(_delete_button, 1, wxALL, 10);
	Bind(wxEVT_BUTTON, &Frame::OnMaterialDelete, this, _delete_button->GetId());
	_sizer->Add(hsizer, 0, wxALL | wxEXPAND, 10);
};

void p6::MaterialBar::show()
{
	_sizer = new wxBoxSizer(wxVERTICAL);
	//Material choice
	_sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Material:"), 0, wxLEFT | wxEXPAND, 10);
	_name_choice = new wxChoice(_side_panel, wxID_ANY);
	_sizer->Add(_name_choice, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_CHOICE, &Frame::OnMaterialChoice, this, _name_choice->GetId());
	//Name edit
	_sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Name:"), 0, wxLEFT | wxEXPAND, 10);
	_name_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_sizer->Add(_name_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnMaterialName, this, _name_edit->GetId());
	//Nonlinear check
	_nonlinear_check = new wxCheckBox(_side_panel, wxID_ANY, "Non-linear");
	_sizer->Add(_nonlinear_check, 0, wxALL, 10);
	Bind(wxEVT_CHECKBOX, &Frame::OnMaterialNonlinear, this, _nonlinear_check->GetId());
	//Formula edit
	_sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Modulus / formula:"), 0, wxLEFT | wxEXPAND, 10);
	_formula_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_sizer->Add(_formula_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnMaterialFormula, this, _formula_edit->GetId());
	//New-accept-delete buttons
	wxBoxSizer *hsizer = new wxBoxSizer(wxHORIZONTAL);
	_new_button = new wxButton(_side_panel, wxID_ANY, "New");
	hsizer->Add(_new_button, 1, wxALL, 10);
	Bind(wxEVT_BUTTON, &Frame::OnMaterialNew, this, _new_button->GetId());
	_apply_button = new wxButton(_side_panel, wxID_ANY, "Apply");
	hsizer->Add(_apply_button, 1, wxALL, 10);
	Bind(wxEVT_BUTTON, &Frame::OnMaterialApply, this, _apply_button->GetId());
	_delete_button = new wxButton(_side_panel, wxID_ANY, "Delete");
	hsizer->Add(_delete_button, 1, wxALL, 10);
	Bind(wxEVT_BUTTON, &Frame::OnMaterialDelete, this, _delete_button->GetId());
	_sizer->Add(hsizer, 0, wxALL | wxEXPAND, 10);
};

void p6::MaterialBar::OnMaterialChoice(wxCommandEvent &e)
{
	int c = _name_choice->GetSelection();
	if (c == wxNOT_FOUND)
	{
		_name_edit->ChangeValue("");
		_nonlinear_check->SetValue(false);
		_formula_edit->ChangeValue("");
	}
	else
	{
		Material *material = _task.materials[c];
		_name_edit->ChangeValue(material->name());
		bool linear = material->type() == Material::Type::linear;
		_nonlinear_check->SetValue(!linear);
		_formula_edit->ChangeValue(linear ?
			Utils::real_to_string(((LinearMaterial*)material)->modulus()) :
			((NonlinearMaterial*)material)->formula());
	}
};

void p6::MaterialBar::OnMaterialName(wxCommandEvent &e)
{

};

void p6::MaterialBar::OnMaterialNew(wxCommandEvent &e)
{
	_name_choice->SetSelection(wxNOT_FOUND);
	_name_edit->ChangeValue("");
	_nonlinear_check->SetValue(false);
	_formula_edit->ChangeValue("");
};

void p6::MaterialBar::OnMaterialApply(wxCommandEvent &e)
{
	try
	{
		String name = _name_edit->GetValue().ToStdString();
		String formula = _formula_edit->GetValue().ToStdString();
		Material *material;
		if (_nonlinear_check->GetValue())
			material = new NonlinearMaterial(name, formula);
		else
			material = new LinearMaterial(name, Utils::string_to_real(formula));

		int c = _name_choice->FindString(name, true);
		if (c == wxNOT_FOUND)
		{
			_task.materials.push_back(material);
			_refresh_materials();
		}
		else
		{
			delete _task.materials[c];
			_task.materials[c] = material;
		}
	}
	catch (std::exception &e)
	{
		wxMessageBox(e.what(), "Error", wxICON_ERROR, this);
	}
};

void p6::MaterialBar::OnMaterialDelete(wxCommandEvent &e)
{
	int c = _name_choice->GetSelection();
	if (c != wxNOT_FOUND)
	{
		for (unsigned int i = 0; i < _task.sticks.size(); i++)
		{
			if (_task.sticks[i].material_index == c)
				_task.sticks[i].material_index = (unsigned int)-1;
		}
		delete _task.materials[c];
		_task.materials.erase(_task.materials.cbegin() + c);
		_refresh_materials();
	}
};

void p6::MaterialBar::OnMaterialNonlinear(wxCommandEvent &e)
{
};

void p6::MaterialBar::OnMaterialFormula(wxCommandEvent &e)
{
};