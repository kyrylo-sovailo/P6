#include "../header/p6_frame.h"
#include "../header/p6_utils.h"
#include <set>

void p6::ForceBar::_on_force_x(wxCommandEvent &e)
{
	real x = Utils::string_to_real(_x_text->GetValue().ToStdString());
	if (x == x)
	{
		std::set<size_t> *selected_forces = &_frame->main_panel.selected_forces;
		for (auto i = selected_forces->cbegin(); i != selected_forces->cend(); i++)
			_frame->construction.set_force_x(*i, x);
		_frame->main_panel.refresh();
	}
};

void p6::ForceBar::_on_force_y(wxCommandEvent &e)
{
	real y = Utils::string_to_real(_y_text->GetValue().ToStdString());
	if (y == y)
	{
		std::set<size_t> *selected_forces = &_frame->main_panel.selected_forces;
		for (auto i = selected_forces->cbegin(); i != selected_forces->cend(); i++)
			_frame->construction.set_force_y(*i, y);
		_frame->main_panel.refresh();
	}
};

p6::ForceBar::ForceBar(Frame *frame)
{
	_frame = frame;
	wxWindow *parent = frame->side_panel.panel;
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
};

void p6::ForceBar::show()
{
	std::set<size_t> *selected_forces = &_frame->main_panel.selected_forces;

	//Set x
	bool x_equal = true;
	real x_value = _frame->construction.get_force_x(*selected_forces->cbegin());
	for (auto i = ++selected_forces->cbegin(); i != selected_forces->cend(); i++)
	{
		if (x_value != _frame->construction.get_force_x(*i)) { x_equal = false; break; }
	}
	if (x_equal) _x_text->ChangeValue(Utils::real_to_string(x_value));
	else _x_text->ChangeValue("");

	//Set y
	bool y_equal = true;
	real y_value = _frame->construction.get_force_y(*selected_forces->cbegin());
	for (auto i = ++selected_forces->cbegin(); i != selected_forces->cend(); i++)
	{
		if (y_value != _frame->construction.get_force_y(*i)) { y_equal = false; break; }
	}
	if (y_equal) _y_text->ChangeValue(Utils::real_to_string(y_value));
	else _y_text->ChangeValue("");

	//Add to sizer
	wxBoxSizer *sizer = _frame->side_panel.sizer;
	sizer->Add(_x_static, 0, wxEXPAND | wxALL, 10);
	sizer->Add(_x_text, 0, wxEXPAND | wxALL, 10);
	sizer->Add(_y_static, 0, wxEXPAND | wxALL, 10);
	sizer->Add(_y_text, 0, wxEXPAND | wxALL, 10);
};

void p6::ForceBar::hide()
{};