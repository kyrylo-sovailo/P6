#include "../header/p6_frame.h"
#include "../header/p6_utils.h"
#include "../header/p6_linear_material.h"
#include "../header/p6_nonlinear_material.h"
#include <algorithm>

void p6::Frame::_init_menubar()
{
	wxMenuBar *menubar = new wxMenuBar();
	
	//File
	wxMenu *filemenu = new wxMenu();
	wxMenuItem *item = filemenu->Append(wxID_ANY, "Load", "Load construction from file");
	Bind(wxEVT_MENU, &Frame::OnMenuFileLoad, this, item->GetId());
	item = filemenu->Append(wxID_ANY, "Import", "Import construction from file");
	Bind(wxEVT_MENU, &Frame::OnMenuFileImport, this, item->GetId());
	item = filemenu->Append(wxID_ANY, "Save", "Save construction to file");
	Bind(wxEVT_MENU, &Frame::OnMenuFileSave, this, item->GetId());
	item = filemenu->Append(wxID_ANY, "Save as", "Save construction another file");
	Bind(wxEVT_MENU, &Frame::OnMenuFileSaveAs, this, item->GetId());
	menubar->Append(filemenu, "File");

	//Export
	wxMenu *exportmenu = new wxMenu();
	item = exportmenu->Append(wxID_ANY, "PNG", "Export image as PNG with white background");
	Bind(wxEVT_MENU, &Frame::OnMenuExportPNG, this, item->GetId());
	item = exportmenu->Append(wxID_ANY, "JPEG", "Export image as JPEG");
	Bind(wxEVT_MENU, &Frame::OnMenuExportJPEG, this, item->GetId());
	menubar->Append(exportmenu, "Export");

	//Help
	wxMenu *helpmenu = new wxMenu();
	item = helpmenu->Append(wxID_ANY, "Help", "Get help");
	Bind(wxEVT_MENU, &Frame::OnMenuHelp, this, item->GetId());
	menubar->Append(helpmenu, "Help");

	SetMenuBar(menubar);
};

void p6::Frame::_init_toolbar()
{
	//Note: not sure what object should Bind be called on. I'll call it on frame
	wxToolBar *toolbar = CreateToolBar(wxID_ANY, wxID_ANY, "Toolbar");

	//Simulate tool
	_toolbar.simulate_tool = toolbar->AddCheckTool(
		wxID_ANY,
		"Simulate",
		Utils::load_png("icons/simulate.png"),
		Utils::load_png("icons/simulate_disabled.png"),
		"Run simulation",
		"Run simulation of the designed construction");
	Bind(wxEVT_TOOL, &Frame::OnToolSimulate, this, _toolbar.simulate_tool->GetId());

	//Select tool
	_toolbar.select_tool = toolbar->AddCheckTool(
		wxID_ANY,
		"Select",
		Utils::load_png("icons/select.png"),
		Utils::load_png("icons/select_disabled.png"),
		"Select items",
		"Select nodes, sticks or forces one by one");
	toolbar->Bind(wxEVT_TOOL, &Frame::OnToolSelect, this, _toolbar.select_tool->GetId());

	//Area tool
	_toolbar.area_tool = toolbar->AddCheckTool(
		wxID_ANY,
		"Area",
		Utils::load_png("icons/area.png"),
		Utils::load_png("icons/area_disabled.png"),
		"Select area",
		"Select all nodes, sticks or forces in area");
	toolbar->Bind(wxEVT_TOOL, &Frame::OnToolArea, this, _toolbar.area_tool->GetId());

	//Move tool
	_toolbar.move_tool = toolbar->AddCheckTool(
		wxID_ANY,
		"Move",
		Utils::load_png("icons/move.png"),
		Utils::load_png("icons/move_disabled.png"),
		"Move nodes",
		"Complex move of selected nodes");
	toolbar->Bind(wxEVT_TOOL, &Frame::OnToolMove, this, _toolbar.move_tool->GetId());

	//Node tool
	_toolbar.node_tool = toolbar->AddCheckTool(
		wxID_ANY,
		"Node",
		Utils::load_png("icons/node.png"),
		Utils::load_png("icons/node_disabled.png"),
		"Create node",
		"Create node or select nodes only");
	toolbar->Bind(wxEVT_TOOL, &Frame::OnToolNode, this, _toolbar.node_tool->GetId());

	//Stick tool
	_toolbar.stick_tool = toolbar->AddCheckTool(
		wxID_ANY,
		"Stick",
		Utils::load_png("icons/stick.png"),
		Utils::load_png("icons/stick_disabled.png"),
		"Create stick",
		"Create stick between selected nodes or select sticks only");
	toolbar->Bind(wxEVT_TOOL, &Frame::OnToolStick, this, _toolbar.stick_tool->GetId());

	//Force tool
	_toolbar.force_tool = toolbar->AddCheckTool(
		wxID_ANY,
		"Force",
		Utils::load_png("icons/force.png"),
		Utils::load_png("icons/force_disabled.png"),
		"Create force",
		"Create force on selected node or select nodes only");
	toolbar->Bind(wxEVT_TOOL, &Frame::OnToolForce, this, _toolbar.force_tool->GetId());

	//Delete tool
	_toolbar.delete_tool = toolbar->AddCheckTool(
		wxID_ANY,
		"Delete",
		Utils::load_png("icons/delete.png"),
		Utils::load_png("icons/delete_disabled.png"),
		"Delete",
		"Delete selected items");
	toolbar->Bind(wxEVT_TOOL, &Frame::OnToolDelete, this, _toolbar.delete_tool->GetId());
	toolbar->Realize();
};

void p6::Frame::_init_node_bar()
{
	_node_bar.sizer = new wxBoxSizer(wxVERTICAL);
	//Free/fixed check
	_node_bar.free_check = new wxCheckBox(_side_panel, wxID_ANY, "Free");
	_node_bar.sizer->Add(_node_bar.free_check, 0, wxALL, 10);
	Bind(wxEVT_CHECKBOX, &Frame::OnNodeFree, this, _node_bar.free_check->GetId());
	_node_bar.fixed_check = new wxCheckBox(_side_panel, wxID_ANY, "Fixed");
	_node_bar.sizer->Add(_node_bar.fixed_check, 0, wxALL, 10);
	Bind(wxEVT_CHECKBOX, &Frame::OnNodeFixed, this, _node_bar.fixed_check->GetId());
	//X/Y edits
	_node_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "X:"), 0, wxLEFT | wxEXPAND, 10);
	_node_bar.x_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_node_bar.sizer->Add(_node_bar.x_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnNodeX, this, _node_bar.x_edit->GetId());
	_node_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Y:"), 0, wxLEFT | wxEXPAND, 10);
	_node_bar.y_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_node_bar.sizer->Add(_node_bar.y_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnNodeY, this, _node_bar.y_edit->GetId());
	//Hide
	_node_bar.sizer->ShowItems(false);
	_node_bar.sizer->Show(false);
};

void p6::Frame::_init_stick_bar()
{
	_stick_bar.sizer = new wxBoxSizer(wxVERTICAL);
	//Material choice
	_stick_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Material:"), 0, wxLEFT | wxEXPAND, 10);
	_stick_bar.material_choice = new wxChoice(_side_panel, wxID_ANY);
	_stick_bar.sizer->Add(_stick_bar.material_choice, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_CHOICE, &Frame::OnStickMaterial, this, _stick_bar.material_choice->GetId());
	//Area edit
	_stick_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Cross-sectional area:"), 0, wxLEFT | wxEXPAND, 10);
	_stick_bar.area_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_stick_bar.sizer->Add(_stick_bar.area_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnStickArea, this, _stick_bar.area_edit->GetId());
	//Length, strain, force
	_stick_bar.length_text = new wxStaticText(_side_panel, wxID_ANY, "");
	_stick_bar.sizer->Add(_stick_bar.length_text, wxALL | wxEXPAND, 10);
	_stick_bar.strain_text = new wxStaticText(_side_panel, wxID_ANY, "");
	_stick_bar.sizer->Add(_stick_bar.strain_text, wxALL | wxEXPAND, 10);
	_stick_bar.force_text = new wxStaticText(_side_panel, wxID_ANY, "");
	_stick_bar.sizer->Add(_stick_bar.force_text, wxALL | wxEXPAND, 10);
	
	//Hide
	_stick_bar.sizer->ShowItems(false);
	_stick_bar.sizer->Show(false);
};

void p6::Frame::_init_force_bar()
{
	_force_bar.sizer = new wxBoxSizer(wxVERTICAL);
	//X/Y edits
	_force_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "X:"), 0, wxLEFT | wxEXPAND, 10);
	_force_bar.x_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_force_bar.sizer->Add(_force_bar.x_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnForceX, this, _force_bar.x_edit->GetId());
	_force_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Y:"), 0, wxLEFT | wxEXPAND, 10);
	_force_bar.y_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_force_bar.sizer->Add(_force_bar.y_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnForceY, this, _force_bar.y_edit->GetId());
	//Hide
	_force_bar.sizer->ShowItems(false);
	_force_bar.sizer->Show(false);
};

void p6::Frame::_init_material_bar()
{
	_material_bar.sizer = new wxBoxSizer(wxVERTICAL);
	//Material choice
	_material_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Material:"), 0, wxLEFT | wxEXPAND, 10);
	_material_bar.name_choice = new wxChoice(_side_panel, wxID_ANY);
	_material_bar.sizer->Add(_material_bar.name_choice, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_CHOICE, &Frame::OnMaterialChoice, this, _material_bar.name_choice->GetId());
	//Name edit
	_material_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Name:"), 0, wxLEFT | wxEXPAND, 10);
	_material_bar.name_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_material_bar.sizer->Add(_material_bar.name_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnMaterialName, this, _material_bar.name_edit->GetId());
	//Nonlinear check
	_material_bar.nonlinear_check = new wxCheckBox(_side_panel, wxID_ANY, "Non-linear");
	_material_bar.sizer->Add(_material_bar.nonlinear_check, 0, wxALL, 10);
	Bind(wxEVT_CHECKBOX, &Frame::OnMaterialNonlinear, this, _material_bar.nonlinear_check->GetId());
	//Formula edit
	_material_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Modulus / formula:"), 0, wxLEFT | wxEXPAND, 10);
	_material_bar.formula_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_material_bar.sizer->Add(_material_bar.formula_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnMaterialFormula, this, _material_bar.formula_edit->GetId());
	//New-accept-delete buttons
	wxBoxSizer *hsizer = new wxBoxSizer(wxHORIZONTAL);
	_material_bar.new_button = new wxButton(_side_panel, wxID_ANY, "New");
	hsizer->Add(_material_bar.new_button, 1, wxALL, 10);
	Bind(wxEVT_BUTTON, &Frame::OnMaterialNew, this, _material_bar.new_button->GetId());
	_material_bar.apply_button = new wxButton(_side_panel, wxID_ANY, "Apply");
	hsizer->Add(_material_bar.apply_button, 1, wxALL, 10);
	Bind(wxEVT_BUTTON, &Frame::OnMaterialApply, this, _material_bar.apply_button->GetId());
	_material_bar.delete_button = new wxButton(_side_panel, wxID_ANY, "Delete");
	hsizer->Add(_material_bar.delete_button, 1, wxALL, 10);
	Bind(wxEVT_BUTTON, &Frame::OnMaterialDelete, this, _material_bar.delete_button->GetId());
	_material_bar.sizer->Add(hsizer, 0, wxALL | wxEXPAND, 10);
};

void p6::Frame::_init_move_bar()
{
	_move_bar.sizer = new wxBoxSizer(wxVERTICAL);
	//Anchor X/Y
	_move_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Anchor X:"), 0, wxLEFT | wxEXPAND, 10);
	_move_bar.anchor_x_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_move_bar.sizer->Add(_move_bar.anchor_x_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnMoveAnchorX, this, _move_bar.anchor_x_edit->GetId());
	_move_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Anchor Y:"), 0, wxLEFT | wxEXPAND, 10);
	_move_bar.anchor_y_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_move_bar.sizer->Add(_move_bar.anchor_y_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnMoveAnchorY, this, _move_bar.anchor_y_edit->GetId());
	//Scale
	_move_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Scale:"), 0, wxLEFT | wxEXPAND, 10);
	_move_bar.scale_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_move_bar.sizer->Add(_move_bar.scale_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnMoveScale, this, _move_bar.scale_edit->GetId());
	//Angle
	_move_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Angle:"), 0, wxLEFT | wxEXPAND, 10);
	_move_bar.angle_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_move_bar.sizer->Add(_move_bar.angle_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnMoveAngle, this, _move_bar.angle_edit->GetId());
	//Shift X/Y
	_move_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Shift X:"), 0, wxLEFT | wxEXPAND, 10);
	_move_bar.shift_x_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_move_bar.sizer->Add(_move_bar.shift_x_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnMoveShiftX, this, _move_bar.shift_x_edit->GetId());
	_move_bar.sizer->Add(new wxStaticText(_side_panel, wxID_ANY, "Shift Y:"), 0, wxLEFT | wxEXPAND, 10);
	_move_bar.shift_y_edit = new wxTextCtrl(_side_panel, wxID_ANY);
	_move_bar.sizer->Add(_move_bar.shift_y_edit, 0, wxALL | wxEXPAND, 10);
	Bind(wxEVT_TEXT, &Frame::OnMoveShiftY, this, _move_bar.shift_y_edit->GetId());
	//Hide
	_move_bar.sizer->ShowItems(false);
	_move_bar.sizer->Show(false);
};

void p6::Frame::_refresh_side_panel()
{
	if (_tool == Tool::move)
	{
		_side_panel->SetSizer(_move_bar.sizer, false);
	}
	else if (!_selection.nodes.empty() && _selection.sticks.empty() && _selection.forces.empty())
	{
		//Node bar
		//Setting free/fixed
		Construction *con = (_tool == Tool::simulate) ? &_result : &_task;

		bool free_equal = true;
		bool free_value = con->nodes[*_selection.nodes.cbegin()].free;
		for (auto i = ++_selection.nodes.cbegin(); i != _selection.nodes.cend(); i++)
		{
			if (free_value != con->nodes[*i].free) { free_equal = false; break; }
		}
		_node_bar.free_check->SetValue(!free_equal || free_value);
		_node_bar.fixed_check->SetValue(!free_equal || !free_value);

		//Setting x
		bool x_equal = true;
		real x_value = con->nodes[*_selection.nodes.cbegin()].x;
		for (auto i = ++_selection.nodes.cbegin(); i != _selection.nodes.cend(); i++)
		{
			if (x_value != con->nodes[*i].x) { x_equal = false; break; }
		}
		if (x_equal) _node_bar.x_edit->ChangeValue(Utils::real_to_string(x_value));
		else _node_bar.x_edit->ChangeValue("");

		//Setting y
		bool y_equal = true;
		real y_value = con->nodes[*_selection.nodes.cbegin()].y;
		for (auto i = ++_selection.nodes.cbegin(); i != _selection.nodes.cend(); i++)
		{
			if (y_value != con->nodes[*i].y) { y_equal = false; break; }
		}
		if (y_equal) _node_bar.y_edit->ChangeValue(Utils::real_to_string(y_value));
		else _node_bar.y_edit->ChangeValue("");

		_side_panel->SetSizer(_node_bar.sizer, false);
	}
	else if (!_selection.sticks.empty() && _selection.nodes.empty() && _selection.forces.empty())
	{
		//Stick bar
		//Setting area
		bool area_equal = true;
		real area_value = _task.sticks[*_selection.sticks.cbegin()].area;
		for (auto i = ++_selection.sticks.cbegin(); i != _selection.sticks.cend(); i++)
		{
			if (area_value != _task.sticks[*i].area) { area_equal = false; break; }
		}
		if (area_equal) _stick_bar.area_edit->ChangeValue(Utils::real_to_string(area_value));
		else _stick_bar.area_edit->ChangeValue("");

		//Setting material
		bool material_equal = true;
		unsigned int material_value = _task.sticks[*_selection.sticks.cbegin()].material_index;
		for (auto i = ++_selection.sticks.cbegin(); i != _selection.sticks.cend(); i++)
		{
			if (material_value != _task.sticks[*i].material_index) { material_equal = false; break; }
		}
		if (!material_equal) _stick_bar.material_choice->SetSelection(wxNOT_FOUND);
		else _stick_bar.material_choice->SetSelection(material_value);
		
		//Setting length
		bool sim = _tool == Tool::simulate;
		bool length_equal = true;
		real length_value = _stick_length(sim, *_selection.sticks.cbegin());
		for (auto i = ++_selection.sticks.cbegin(); i != _selection.sticks.cend(); i++)
		{
			if (length_value != _stick_length(sim, *i)) { length_equal = false; break; }
		}
		wxStaticText *newtext;
		if (length_equal) newtext = new wxStaticText(_side_panel, wxID_ANY, "Length: " + Utils::real_to_string(length_value));
		else newtext = new wxStaticText(_side_panel, wxID_ANY, "Length: ");
		_stick_bar.sizer->Replace(_stick_bar.length_text, newtext);
		delete _stick_bar.length_text;
		_stick_bar.length_text = newtext;

		//Setting strain
		if (_tool == Tool::simulate)
		{
			bool strain_equal = true;
			real strain_value = _stick_strain(*_selection.sticks.cbegin());
			for (auto i = ++_selection.sticks.cbegin(); i != _selection.sticks.cend(); i++)
			{
				if (strain_value != _stick_strain(*i)) { strain_equal = false; break; }
			}
			if (strain_equal) newtext = new wxStaticText(_side_panel, wxID_ANY, "Strain: " + Utils::real_to_string(strain_value));
			else newtext = new wxStaticText(_side_panel, wxID_ANY, "Strain: ");
		}
		else newtext = new wxStaticText(_side_panel, wxID_ANY, "Strain: ");
		_stick_bar.sizer->Replace(_stick_bar.strain_text, newtext);
		delete _stick_bar.strain_text;
		_stick_bar.strain_text = newtext;

		//Setting force
		if (_tool == Tool::simulate)
		{
			bool force_equal = true;
			real force_value = _stick_force(*_selection.sticks.cbegin());
			for (auto i = ++_selection.sticks.cbegin(); i != _selection.sticks.cend(); i++)
			{
				if (force_value != _stick_force(*i)) { force_equal = false; break; }
			}
			if (force_equal) newtext = new wxStaticText(_side_panel, wxID_ANY, "Force: " + Utils::real_to_string(force_value));
			else newtext = new wxStaticText(_side_panel, wxID_ANY, "Force: ");
		}
		else newtext = new wxStaticText(_side_panel, wxID_ANY, "Force: ");
		_stick_bar.sizer->Replace(_stick_bar.force_text, newtext);
		delete _stick_bar.force_text;
		_stick_bar.force_text = newtext;

		_side_panel->SetSizer(_stick_bar.sizer, false);
	}
	else if (!_selection.forces.empty() && _selection.nodes.empty() && _selection.sticks.empty())
	{
		//Force bar
		//Setting x
		bool x_equal = true;
		real x_value = _task.nodes[*_selection.forces.cbegin()].xforce;
		for (auto i = _selection.nodes.cbegin(); i != _selection.nodes.cend(); i++)
		{
			if (x_value != _task.nodes[*i].xforce) { x_equal = false; break; }
		}
		if (x_equal) _force_bar.x_edit->ChangeValue(Utils::real_to_string(x_value));
		else _force_bar.x_edit->ChangeValue("");

		//Setting y
		bool y_equal = true;
		real y_value = _task.nodes[*_selection.forces.cbegin()].yforce;
		for (auto i = _selection.nodes.cbegin(); i != _selection.nodes.cend(); i++)
		{
			if (y_value != _task.nodes[*i].yforce) { y_equal = false; break; }
		}
		if (y_equal) _force_bar.y_edit->ChangeValue(Utils::real_to_string(y_value));
		else _force_bar.y_edit->ChangeValue("");

		_side_panel->SetSizer(_force_bar.sizer, false);
	}
	else
	{
		//Material bar
		_material_bar.name_choice->SetSelection(wxNOT_FOUND);
		_material_bar.name_edit->ChangeValue("");
		_material_bar.nonlinear_check->SetValue(false);
		_material_bar.formula_edit->ChangeValue("");

		_side_panel->SetSizer(_material_bar.sizer, false);
	}

	_node_bar.sizer->Show(_side_panel->GetSizer() == _node_bar.sizer);
	_node_bar.sizer->ShowItems(_side_panel->GetSizer() == _node_bar.sizer);
	_stick_bar.sizer->Show(_side_panel->GetSizer() == _stick_bar.sizer);
	_stick_bar.sizer->ShowItems(_side_panel->GetSizer() == _stick_bar.sizer);
	_force_bar.sizer->Show(_side_panel->GetSizer() == _force_bar.sizer);
	_force_bar.sizer->ShowItems(_side_panel->GetSizer() == _force_bar.sizer);
	_material_bar.sizer->Show(_side_panel->GetSizer() == _material_bar.sizer);
	_material_bar.sizer->ShowItems(_side_panel->GetSizer() == _material_bar.sizer);
	_move_bar.sizer->Show(_side_panel->GetSizer() == _move_bar.sizer);
	_move_bar.sizer->ShowItems(_side_panel->GetSizer() == _move_bar.sizer);
	_side_panel->Layout();
};

void p6::Frame::_refresh_toolbar()
{
	wxToolBar *toolbar = GetToolBar();
	toolbar->ToggleTool(_toolbar.simulate_tool->GetId(), _tool == Tool::simulate);
	toolbar->ToggleTool(_toolbar.select_tool->GetId(), _tool == Tool::select);
	toolbar->ToggleTool(_toolbar.area_tool->GetId(), _tool == Tool::area);
	toolbar->ToggleTool(_toolbar.move_tool->GetId(), _tool == Tool::move);
		toolbar->EnableTool(_toolbar.move_tool->GetId(), _tool != Tool::simulate);
	toolbar->ToggleTool(_toolbar.node_tool->GetId(), _tool == Tool::node);
	toolbar->ToggleTool(_toolbar.stick_tool->GetId(), _tool == Tool::stick);
	toolbar->ToggleTool(_toolbar.force_tool->GetId(), _tool == Tool::force);
	toolbar->ToggleTool(_toolbar.delete_tool->GetId(), _tool == Tool::delet);
		toolbar->EnableTool(_toolbar.delete_tool->GetId(), _tool != Tool::simulate);
};

void p6::Frame::_refresh_materials()
{
	wxArrayString array;
	array.Alloc(_task.materials.size());
	for (unsigned int i = 0; i < _task.materials.size(); i++)
		array.Add(_task.materials[i]->name());

	//Replacing in material bar
	Unbind(wxEVT_CHOICE, &Frame::OnMaterialChoice, this, _material_bar.name_choice->GetId());
	wxChoice *newchoice = new wxChoice(_side_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, array);
	_material_bar.sizer->Replace(_material_bar.name_choice, newchoice);
	_material_bar.name_choice->Destroy();
	_material_bar.name_choice = newchoice;
	Bind(wxEVT_CHOICE, &Frame::OnMaterialChoice, this, _material_bar.name_choice->GetId());

	//Replacing in stick bar
	Unbind(wxEVT_CHOICE, &Frame::OnStickMaterial, this, _stick_bar.material_choice->GetId());
	newchoice = new wxChoice(_side_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, array);
	_stick_bar.sizer->Replace(_stick_bar.material_choice, newchoice);
	_stick_bar.material_choice->Destroy();
	_stick_bar.material_choice = newchoice;
	Bind(wxEVT_CHOICE, &Frame::OnStickMaterial, this, _stick_bar.material_choice->GetId());

	_refresh_side_panel();
};

void p6::Frame::_render(wxDC *dc, bool panel)
{
	bool sim = (_tool == Tool::simulate);

	//Draw sticks
	for (size_t i = 0; i < _task.sticks.size(); i++)
	{
		const unsigned int *node_index = _task.sticks[i].node_index;

		wxColour colour;
		if (sim)
		{
			real strain = _stick_strain(i) / _max_strain;

			if (strain > 0)
			{
				real rg = std::max(128.0 - 128.0 * strain, 0.0);
				real b = std::min(128.0 + 127 * strain, 255.0);
				colour = wxColour(rg, rg, b);
			}
			else
			{
				//Red
				real gb = std::max(128.0 + 128 * strain, 0.0);
				real r = std::min(128.0 - 127.0 * strain, 255.0);
				colour = wxColour(r, gb, gb);
			}
		}
		else colour = wxColour(128, 128, 128);

		wxPoint points[2];
		for (size_t j = 0; j < 2; j++)
		{
			const Node *node = &(sim ? _result : _task).nodes[node_index[j]];
			points[j].x = _real_to_pixel_x(node->x, panel);
			points[j].y = _real_to_pixel_y(node->y, panel);
		}
		if (_selection.sticks.count(i))
		{
			dc->SetPen(wxPen(wxColour(255, 255, 0), 4));
			dc->DrawLine(points[0], points[1]);
		}
		dc->SetPen(wxPen(colour, 3));
		dc->DrawLine(points[0], points[1]);
	}

	//Draw forces and points
	for (size_t i = 0; i < _task.nodes.size(); i++)
	{
		Node *node = &(sim ? _result : _task).nodes[i];
		wxPoint point;
		point.x = _real_to_pixel_x(node->x, panel);
		point.y = _real_to_pixel_y(node->y, panel);
		//Force
		if (node->xforce != 0 || node->yforce != 0)
		{
			const real a = 30.0 * (M_PI / 180.0);
			const real c = 0.1;

			real bax = -node->xforce * _meters_in_newton;
			real bay = -node->yforce * _meters_in_newton;
			real bcx = c * (cos(a) * bax - sin(a) * bay);
			real bcy = c * (sin(a) * bax + cos(a) * bay);
			real bdx = c * (cos(a) * bax + sin(a) * bay);
			real bdy = c * (-sin(a) * bax + cos(a) * bay);
			wxPoint pb = wxPoint(_real_to_pixel_x(node->x - bax, panel), _real_to_pixel_y(node->y - bay, panel));
			wxPoint pc = wxPoint(_real_to_pixel_x(node->x - bax + bcx, panel), _real_to_pixel_y(node->y - bay + bcy, panel));
			wxPoint pd = wxPoint(_real_to_pixel_x(node->x - bax + bdx, panel), _real_to_pixel_y(node->y - bay + bdy, panel));
			if (_selection.forces.count(i))
			{
				dc->SetPen(wxPen(wxColour(255, 255, 0), 2));
				dc->DrawLine(point, pb);
				dc->DrawLine(pb, pc);
				dc->DrawLine(pb, pd);
			}
			dc->SetPen(wxPen(wxColour(0, 255, 0), 1));
			dc->DrawLine(point, pb);
			dc->DrawLine(pb, pc);
			dc->DrawLine(pb, pd);
		}
		//Node
		if (_selection.nodes.count(i)) dc->SetPen(wxPen(wxColour(255, 255, 0), 1));
		else dc->SetPen(wxPen(wxColour(0, 0, 0), 1));
		if (_task.nodes[i].free) dc->SetBrush(wxBrush(wxColour(128, 128, 128)));
		else dc->SetBrush(wxBrush(wxColour(64, 64, 64)));
		dc->DrawCircle(point, 5);
	}

	//Draw selection rect
	if (_tool == Tool::area && _mouse.state == Mouse::move_on_panel)
	{
		dc->SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SHORT_DASH));
		wxPoint p[2];
		p[0].x = _real_to_pixel_x(_pixel_to_real_x(_mouse.xdown), panel);
		p[0].y = _real_to_pixel_y(_pixel_to_real_y(_mouse.ydown), panel);
		p[1].x = p[0].x + _mouse.xup - _mouse.xdown;
		p[1].y = p[0].y + _mouse.yup - _mouse.ydown;
		dc->DrawLine(p[0], wxPoint(p[0].x, p[1].y));
		dc->DrawLine(wxPoint(p[0].x, p[1].y), p[1]);
		dc->DrawLine(p[1], wxPoint(p[1].x, p[0].y));
		dc->DrawLine(wxPoint(p[1].x, p[0].y), p[0]);
	}
};

p6::real p6::Frame::_pixel_to_real_x(wxCoord x) const
{
	return _x + (real)(x - _main_panel->GetSize().x / 2) / _pixels_in_meter;
};

p6::real p6::Frame::_pixel_to_real_y(wxCoord y) const
{
	return _y + (real)(_main_panel->GetSize().y / 2 - y) / _pixels_in_meter;
};

wxCoord p6::Frame::_real_to_pixel_x(real x, bool panel) const
{
	return ((panel ? GetClientAreaOrigin().x : 0) +
		_main_panel->GetSize().x / 2 + (x - _x) * _pixels_in_meter);
};

wxCoord p6::Frame::_real_to_pixel_y(real y, bool panel) const
{
	return ((panel ? GetClientAreaOrigin().y : 0) +
		_main_panel->GetSize().y / 2 - (y - _y) * _pixels_in_meter);
};

p6::real p6::Frame::_stick_length(bool sim, unsigned int i)	const
{
	const Construction *con = (sim ? &_result : &_task);
	const unsigned int *node_index = _task.sticks[i].node_index;
	return sqrt(
		sqr(con->nodes[node_index[1]].x - con->nodes[node_index[0]].x) +
		sqr(con->nodes[node_index[1]].y - con->nodes[node_index[0]].y));
};

p6::real p6::Frame::_stick_strain(unsigned int i) const
{
	return _stick_length(true, i) / _stick_length(false, i) - 1.0;
};

p6::real p6::Frame::_stick_force(unsigned int i) const
{
	return _task.sticks[i].area * _task.materials[_task.sticks[i].material_index]->stress(_stick_strain(i));
};

void p6::Frame::_node_delete(unsigned int index)
{
	for (unsigned int i = _task.sticks.size() - 1; i != (unsigned int)-1; i++)
	{
		if (_task.sticks[i].node_index[0] == index || _task.sticks[i].node_index[1] == index)
		{
			_task.sticks.erase(_task.sticks.cbegin() + i);
		}
		else
		{
			if (_task.sticks[i].node_index[0] > index) _task.sticks[i].node_index[0]--;
			if (_task.sticks[i].node_index[1] > index) _task.sticks[i].node_index[1]--;
		}
	}
	_task.nodes.erase(_task.nodes.cbegin() + index);
};

p6::Frame::Frame() : wxFrame(nullptr, wxID_ANY, "P6")
{
	//Init menubar
	_init_menubar();

	//Init toolbar
	_init_toolbar();

	//Init main panel
	_sizer = new wxBoxSizer(wxHORIZONTAL);
	_main_panel = new wxPanel(this);
	_main_panel->Show(false);
	_sizer->Add(_main_panel, 4, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN, 0);

	//Init side panel
	_side_panel = new wxPanel(this);
	_sizer->Add(_side_panel, 1, wxEXPAND, 0);
	_init_node_bar();
	_init_stick_bar();
	_init_force_bar();
	_init_material_bar();
	_init_move_bar();
	_side_panel->SetSizer(_material_bar.sizer);
	_side_panel->SetMinSize(wxSize(300, -1));
	SetSizer(_sizer);
	
	//Init window handlers
	Bind(wxEVT_LEFT_DOWN, &Frame::OnLeftDown, this, GetId());
	Bind(wxEVT_LEFT_UP, &Frame::OnLeftUp, this, GetId());
	Bind(wxEVT_RIGHT_DOWN, &Frame::OnRightDown, this, GetId());
	Bind(wxEVT_RIGHT_UP, &Frame::OnRightUp, this, GetId());
	Bind(wxEVT_MOTION, &Frame::OnMove, this, GetId());
	Bind(wxEVT_MOUSEWHEEL, &Frame::OnWheel, this, wxID_ANY);
	Bind(wxEVT_PAINT, &Frame::OnPaint, this, GetId());
	Bind(wxEVT_SIZE, &Frame::OnSize, this, GetId());
};

p6::Frame::~Frame()
{
	for (unsigned int i = 0; i < _task.materials.size(); i++)
		delete _task.materials[i];
};

void p6::Frame::OnMenuFileLoad(wxCommandEvent &e)
{
	wxFileDialog dialog(this, "Open P6 File", "", "", "P6 File (*.p6)|*.p6", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		try
		{
			_task = Construction(dialog.GetPath().ToStdString());
			_selection.nodes.clear();
			_selection.sticks.clear();
			_selection.forces.clear();
			_refresh_materials();
			_refresh_side_panel();
			_refresh_toolbar();
			Refresh();
		}
		catch (std::exception &e)
		{
			wxMessageBox(e.what(), "Error", wxICON_ERROR, this);
		}
	}
};

void p6::Frame::OnMenuFileImport(wxCommandEvent &e)
{
	wxFileDialog dialog(this, "Import P6 File", "", "", "P6 File (*.p6)|*.p6", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		try
		{
			unsigned int oldnodes = _task.nodes.size();
			unsigned int oldsticks = _task.sticks.size();
			Construction import = Construction(dialog.GetPath().ToStdString());
			_task.merge(&import);
			_selection.nodes.clear();
			_selection.sticks.clear();
			_selection.forces.clear();
			for (unsigned int i = oldnodes; i < _task.nodes.size(); i++)
			{
				if (_task.nodes[i].xforce != 0 || _task.nodes[i].yforce != 0) _selection.forces.insert(i);
				_selection.nodes.insert(i);
			}
			for (unsigned int i = oldsticks; i < _task.sticks.size(); i++)
				_selection.sticks.insert(i);

			_refresh_materials();
			_refresh_side_panel();
			_refresh_toolbar();
			Refresh();
		}
		catch (std::exception &e)
		{
			wxMessageBox(e.what(), "Error", wxICON_ERROR, this);
		}
	}
};

void p6::Frame::OnMenuFileSave(wxCommandEvent &e)
{
	if (_file_path == "")
	{
		wxFileDialog dialog(this, "Save P6 File", "", "", "P6 File (*.p6)|*.p6", wxFD_SAVE);
		if (dialog.ShowModal() == wxID_CANCEL) return;
		_file_path = dialog.GetPath();
	}

	try
	{
		_task.save(_file_path);
	}
	catch (std::exception &e)
	{
		wxMessageBox(e.what(), "Error", wxICON_ERROR, this);
	}
};

void p6::Frame::OnMenuFileSaveAs(wxCommandEvent &e)
{
	wxFileDialog dialog(this, "Save P6 File as...", "", "", "P6 File (*.p6)|*.p6", wxFD_SAVE);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		try
		{
			_task.save(dialog.GetPath().ToStdString());
		}
		catch (std::exception &e)
		{
			wxMessageBox(e.what(), "Error", wxICON_ERROR, this);
		}
	}
};

void p6::Frame::OnMenuExportPNG(wxCommandEvent &e)
{
	wxFileDialog dialog(this, "Export as PNG", "", "", "PNG Image (*.png)|*.png", wxFD_SAVE);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		wxMemoryDC dc;
		wxBitmap bitmap(_main_panel->GetSize());
		dc.SelectObject(bitmap);
		dc.SetPen(wxPen(wxColour(255, 255, 255), 0));
		dc.SetBrush(wxBrush(wxColour(255, 255, 255)));
		dc.DrawRectangle(wxPoint(0, 0), _main_panel->GetSize());
		_render(&dc, false);
		bitmap.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_PNG);
	}
};

void p6::Frame::OnMenuExportJPEG(wxCommandEvent &e)
{
	wxFileDialog dialog(this, "Export as JPEG", "", "", "JPEG Image (*.jpg)|*.jpg", wxFD_SAVE);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		wxMemoryDC dc;
		wxBitmap bitmap(_main_panel->GetSize());
		dc.SelectObject(bitmap);
		dc.SetPen(wxPen(wxColour(255, 255, 255), 0));
		dc.SetBrush(wxBrush(wxColour(255, 255, 255)));
		dc.DrawRectangle(wxPoint(0, 0), _main_panel->GetSize());
		_render(&dc, false);
		bitmap.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_PNG);
	}
};

void p6::Frame::OnMenuHelp(wxCommandEvent &e)
{
	wxMessageBox("Ask Kyrylo", "Help", wxICON_INFORMATION, this);
};

void p6::Frame::OnToolSimulate(wxCommandEvent &e)
{
	if (_tool == Tool::simulate) _tool = Tool::no;
	else
	{
		_result = _task;
		try
		{
			for (unsigned int i = 0; i < _task.sticks.size(); i++)
			{
				if (_task.sticks[i].material_index >= _task.materials.size())
					throw std::runtime_error("Material is not specified");
			}
			_result.simulate();
			_max_strain = 0.0;
			for (unsigned int i = 0; i < _task.sticks.size(); i++)
			{
				if (abs(_stick_strain(i)) > _max_strain) _max_strain = abs(_stick_strain(i));
			}
			_tool = Tool::simulate;
		}
		catch (std::exception &e)
		{
			wxMessageBox(e.what(), "Error", wxICON_ERROR, this);
		}
	}
	_refresh_toolbar();
	_refresh_side_panel();
	Refresh();
};

void p6::Frame::OnToolSelect(wxCommandEvent &e)
{
	if (_tool == Tool::select) _tool = Tool::no;
	else _tool = Tool::select;
	_selection.nodes.clear();
	_selection.sticks.clear();
	_selection.forces.clear();
	_refresh_toolbar();
	_refresh_side_panel();
	Refresh();
};

void p6::Frame::OnToolArea(wxCommandEvent &e)
{
	if (_tool == Tool::area) _tool = Tool::no;
	else _tool = Tool::area;
	_selection.nodes.clear();
	_selection.sticks.clear();
	_selection.forces.clear();
	_refresh_toolbar();
	_refresh_side_panel();
	Refresh();
};

void p6::Frame::OnToolMove(wxCommandEvent &e)
{
	if (_tool == Tool::move) _tool = Tool::no;
	else _tool = Tool::move;
	_refresh_toolbar();
	_refresh_side_panel();
	Refresh();
};

void p6::Frame::OnToolNode(wxCommandEvent &e)
{
	if (_tool == Tool::node) _tool = Tool::no;
	else if (!_selection.nodes.empty() && (!_selection.sticks.empty() || !_selection.forces.empty()))
	{
		//Choose only nodes
		_selection.sticks.clear();
		_selection.forces.clear();
	}
	else
	{
		//Turn node mode on
		_tool = Tool::node;
		_selection.nodes.clear();
		_selection.sticks.clear();
		_selection.forces.clear();
	}
	_refresh_toolbar();
	_refresh_side_panel();
	Refresh();
};

void p6::Frame::OnToolStick(wxCommandEvent &e)
{
	if (_tool == Tool::stick) _tool = Tool::no;
	else if (_selection.nodes.size() == 2 && _selection.sticks.empty() && _selection.forces.empty())
	{
		//Connect two sticks
		Stick stick;
		stick.area = 0.0;
		stick.material_index = (unsigned int) -1;
		stick.node_index[0] = *_selection.nodes.cbegin();
		stick.node_index[1] = *++_selection.nodes.cbegin();
		_selection.nodes.clear();
		_task.sticks.push_back(stick);
	}
	else if (!_selection.sticks.empty() && (!_selection.nodes.empty() || !_selection.forces.empty()))
	{
		//Choose only sticks
		_selection.nodes.clear();
		_selection.forces.clear();
	}
	else
	{
		//Turn stick mode on
		_tool = Tool::stick; 
		_selection.nodes.clear();
		_selection.sticks.clear();
		_selection.forces.clear();
	}
	_refresh_toolbar();
	_refresh_side_panel();
	Refresh();
};

void p6::Frame::OnToolForce(wxCommandEvent &e)
{
	if (_tool == Tool::force) _tool = Tool::no;
	else if (!_selection.forces.empty() && (!_selection.nodes.empty() || !_selection.sticks.empty()))
	{
		//Choose only forces
		_selection.nodes.clear();
		_selection.sticks.clear();
	}
	else
	{
		//Turn force mode on
		_tool = Tool::force;
		_selection.nodes.clear();
		_selection.sticks.clear();
		_selection.forces.clear();
	}
	_refresh_toolbar();
	_refresh_side_panel();
	Refresh();
};

void p6::Frame::OnToolDelete(wxCommandEvent &e)
{
	if (_tool == Tool::delet) _tool = Tool::no;
	else
	{
		//Delete forces
		for (auto i = _selection.forces.cbegin(); i != _selection.forces.cend(); i++)
		{
			_task.nodes[*i].xforce = 0.0;
			_task.nodes[*i].yforce = 0.0;
		}

		//Delete selected sticks
		for (auto i = _selection.sticks.crbegin(); i != _selection.sticks.crend(); i++)
			_task.sticks.erase(_task.sticks.cbegin() + *i);

		//Delete nodes
		for (auto i = _selection.nodes.cbegin(); i != _selection.nodes.cend(); i++)
			_node_delete(*i);

		_selection.nodes.clear();
		_selection.sticks.clear();
		_selection.forces.clear();
	}
	_refresh_toolbar();
	_refresh_side_panel();
	Refresh();
};

void p6::Frame::OnNodeFree(wxCommandEvent &e)
{
	_node_bar.fixed_check->SetValue(!_node_bar.free_check->GetValue());
	for (auto i = _selection.nodes.cbegin(); i != _selection.nodes.cend(); i++)
		_task.nodes[*i].free = _node_bar.free_check->GetValue();
	Refresh();
};

void p6::Frame::OnNodeFixed(wxCommandEvent &e)
{
	_node_bar.free_check->SetValue(!_node_bar.fixed_check->GetValue());
	for (auto i = _selection.nodes.cbegin(); i != _selection.nodes.cend(); i++)
		_task.nodes[*i].free = _node_bar.free_check->GetValue();
	Refresh();
};

void p6::Frame::OnNodeX(wxCommandEvent &e)
{
	real x = Utils::string_to_real(_node_bar.x_edit->GetValue().ToStdString());
	if (x == x)
	{
		for (auto i = _selection.nodes.cbegin(); i != _selection.nodes.cend(); i++)
			_task.nodes[*i].x = x;
		Refresh();
	}
};

void p6::Frame::OnNodeY(wxCommandEvent &e)
{
	real y = Utils::string_to_real(_node_bar.y_edit->GetValue().ToStdString());
	if (y == y)
	{
		for (auto i = _selection.nodes.cbegin(); i != _selection.nodes.cend(); i++)
			_task.nodes[*i].y = y;
		Refresh();
	}
};

void p6::Frame::OnStickArea(wxCommandEvent &e)
{
	real a = Utils::string_to_real(_stick_bar.area_edit->GetValue().ToStdString());
	if (a == a)
	{
		for (auto i = _selection.sticks.cbegin(); i != _selection.sticks.cend(); i++)
			_task.sticks[*i].area = a;
	}
};

void p6::Frame::OnStickMaterial(wxCommandEvent &e)
{
	int c = _stick_bar.material_choice->GetSelection();
	if (c != wxNOT_FOUND)
	{
		for (auto i = _selection.sticks.cbegin(); i != _selection.sticks.cend(); i++)
			_task.sticks[*i].material_index = c;
	}
};

void p6::Frame::OnForceX(wxCommandEvent &e)
{
	real xforce = Utils::string_to_real(_force_bar.x_edit->GetValue().ToStdString());
	if (xforce == xforce)
	{
		for (auto i = _selection.forces.cbegin(); i != _selection.forces.cend(); i++)
			_task.nodes[*i].xforce = xforce;
		Refresh();
	}
};

void p6::Frame::OnForceY(wxCommandEvent &e)
{
	real yforce = Utils::string_to_real(_force_bar.y_edit->GetValue().ToStdString());
	if (yforce == yforce)
	{
		for (auto i = _selection.forces.cbegin(); i != _selection.forces.cend(); i++)
			_task.nodes[*i].yforce = yforce;
		Refresh();
	}
};

void p6::Frame::OnMaterialChoice(wxCommandEvent &e)
{
	int c = _material_bar.name_choice->GetSelection();
	if (c == wxNOT_FOUND)
	{
		_material_bar.name_edit->ChangeValue("");
		_material_bar.nonlinear_check->SetValue(false);
		_material_bar.formula_edit->ChangeValue("");
	}
	else
	{
		Material *material = _task.materials[c];
		_material_bar.name_edit->ChangeValue(material->name());
		bool linear = material->type() == Material::Type::linear;
		_material_bar.nonlinear_check->SetValue(!linear);
		_material_bar.formula_edit->ChangeValue(linear ?
			Utils::real_to_string(((LinearMaterial*)material)->modulus()) :
			((NonlinearMaterial*)material)->formula());
	}
};

void p6::Frame::OnMaterialName(wxCommandEvent &e)
{

};

void p6::Frame::OnMaterialNew(wxCommandEvent &e)
{
	_material_bar.name_choice->SetSelection(wxNOT_FOUND);
	_material_bar.name_edit->ChangeValue("");
	_material_bar.nonlinear_check->SetValue(false);
	_material_bar.formula_edit->ChangeValue("");
};

void p6::Frame::OnMaterialApply(wxCommandEvent &e)
{
	try
	{
		String name = _material_bar.name_edit->GetValue().ToStdString();
		String formula = _material_bar.formula_edit->GetValue().ToStdString();
		Material *material;
		if (_material_bar.nonlinear_check->GetValue())
			material = new NonlinearMaterial(name, formula);
		else
			material = new LinearMaterial(name, Utils::string_to_real(formula));

		int c = _material_bar.name_choice->FindString(name, true);
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

void p6::Frame::OnMaterialDelete(wxCommandEvent &e)
{
	int c = _material_bar.name_choice->GetSelection();
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

void p6::Frame::OnMaterialNonlinear(wxCommandEvent &e)
{
};

void p6::Frame::OnMaterialFormula(wxCommandEvent &e)
{
};

void p6::Frame::OnMoveAnchorX(wxCommandEvent &e)
{

};

void p6::Frame::OnMoveAnchorY(wxCommandEvent &e)
{};

void p6::Frame::OnMoveScale(wxCommandEvent &e)
{};

void p6::Frame::OnMoveAngle(wxCommandEvent &e)
{};

void p6::Frame::OnMoveShiftX(wxCommandEvent &e)
{};

void p6::Frame::OnMoveShiftY(wxCommandEvent &e)
{

};

void p6::Frame::OnLeftDown(wxMouseEvent &e)
{
	_mouse.xdown = e.GetX();
	_mouse.ydown = e.GetY();
	_mouse.xup = e.GetX();
	_mouse.yup = e.GetY();
	_mouse.xold = _x;
	_mouse.yold = _y;
	bool sim = (_tool == Tool::simulate);
	
	//Checking for clicking node
	for (unsigned int i = 0; i < _task.nodes.size(); i++)
	{
		Node *node = &(sim ? _result : _task).nodes[i];
		if (_pixels_in_meter * sqrt(
			sqr(node->x - _pixel_to_real_x(e.GetX())) +
			sqr(node->y - _pixel_to_real_y(e.GetY()))) < 5.0)
		{
			_mouse.state = Mouse::pressed_on_node;
			_mouse.index = i;
			return;
		}
	}

	//Checking for clicking stick
	real x = _pixel_to_real_x(e.GetX());
	real y = _pixel_to_real_y(e.GetY());
	for (unsigned int i = 0; i < _task.sticks.size(); i++)
	{
		real nx[2], ny[2];
		for (unsigned int j = 0; j < 2; j++)
		{
			nx[j] = (sim ? _result : _task).nodes[_task.sticks[i].node_index[j]].x;
			ny[j] = (sim ? _result : _task).nodes[_task.sticks[i].node_index[j]].y;
		}
		if (Utils::distance_to_line(x, y, nx, ny) *_pixels_in_meter < 5)
		{
			_mouse.state = Mouse::pressed_on_stick;
			_mouse.index = i;
			return;
		}
	}

	//Checking for clicking forces
	for (unsigned int i = 0; i < _task.nodes.size(); i++)
	{
		Node *node = &(sim ? _result : _task).nodes[i];
		if (node->xforce != 0 || node->yforce != 0)
		{
			real nx[2], ny[2];
			nx[0] = node->x;
			ny[0] = node->y;
			nx[1] = node->x + node->xforce * _meters_in_newton;
			ny[1] = node->y + node->yforce * _meters_in_newton;
			if (Utils::distance_to_line(x, y, nx, ny) *_pixels_in_meter < 5)
			{
				_mouse.state = Mouse::pressed_on_force;
				_mouse.index = i;
				return;
			}
		}
	}

	//Nothing found
	_mouse.state = Mouse::pressed_on_panel;
};

void p6::Frame::OnLeftUp(wxMouseEvent &e)
{
	switch (_mouse.state)
	{
	case Mouse::pressed_on_node:
		if (_tool == Tool::delet)
		{
			//Delete one node
			_node_delete(_mouse.index);
		}
		else if (_selection.nodes.count(_mouse.index))
		{
			//Deselect node
			_selection.nodes.erase(_mouse.index);
		}
		else
		{
			//Select node
			_selection.nodes.insert(_mouse.index);
			if (_tool == Tool::stick && _selection.nodes.size() == 2)
			{
				Stick s;
				s.area = 0.0;
				s.material_index = (unsigned int)-1;
				s.node_index[0] = *_selection.nodes.cbegin();
				s.node_index[1] = *++_selection.nodes.cbegin();
				_task.sticks.push_back(s);
				_selection.nodes.clear();
			}
		}
		Refresh();
		_refresh_side_panel();
		break;

	case Mouse::pressed_on_stick:
		if (_tool == Tool::delet)
		{
			//Delete one stick
			_selection.sticks.erase(_mouse.index);
		}
		if (_selection.sticks.count(_mouse.index))
		{
			//Deselect stick
			_selection.sticks.erase(_mouse.index);
		}
		else
		{
			//Seelct stick
			_selection.sticks.insert(_mouse.index);
		}
		Refresh();
		_refresh_side_panel();
		break;

	case Mouse::pressed_on_force:
		if (_tool == Tool::delet)
		{
			//Delete force
			_selection.forces.erase(_mouse.index);
		}
		if (_selection.forces.count(_mouse.index))
		{
			//Deselect force
			_selection.forces.erase(_mouse.index);
		}
		else
		{
			//Select force
			_selection.forces.insert(_mouse.index);
		}
		Refresh();
		_refresh_side_panel();
		break;	

	case Mouse::pressed_on_panel:
		if (_tool == Tool::node)
		{
			//Create node
			Node node;
			node.free = true;
			node.x = _pixel_to_real_x(e.GetX());
			node.y = _pixel_to_real_y(e.GetY());
			node.xforce = 0.0;
			node.yforce = 0.0;
			_task.nodes.push_back(node);
			Refresh();
		}
		break;

	case Mouse::move_on_panel:
		if (_tool == Tool::area)
		{
			//Select items
			_selection.nodes.clear();
			_selection.sticks.clear();
			_selection.forces.clear();
			real x[2], y[2];
			x[0] = _pixel_to_real_x(e.GetX());
			x[1] = _pixel_to_real_x(_mouse.xdown);
			if (x[1] < x[0]) { real b = x[0]; x[0] = x[1]; x[1] = b; }
			y[0] = _pixel_to_real_y(e.GetY());
			y[1] = _pixel_to_real_y(_mouse.ydown);
			if (y[1] < y[0]) { real b = y[0]; y[0] = y[1]; y[1] = b; }
			for (unsigned int i = 0; i < _task.nodes.size(); i++)
			{
				if(x[0] < _task.nodes[i].x && _task.nodes[i].x < x[1]
				&& y[0] < _task.nodes[i].y && _task.nodes[i].y < y[1])
					_selection.nodes.insert(i);
			}
			for (unsigned int i = 0; i < _task.sticks.size(); i++)
			{
				if(_selection.nodes.count(_task.sticks[i].node_index[0])
				&& _selection.nodes.count(_task.sticks[i].node_index[1]))
					_selection.sticks.insert(i);
			}
			_refresh_side_panel();
			Refresh();
		}
		break;

	case Mouse::move_on_node:
		if (_tool == Tool::force)
		{
			//Modify force
			real x = _pixel_to_real_x(e.GetX());
			real y = _pixel_to_real_y(e.GetY());
			_task.nodes[_mouse.index].xforce = (x - _task.nodes[_mouse.index].x) / _meters_in_newton;
			_task.nodes[_mouse.index].yforce = (y - _task.nodes[_mouse.index].y) / _meters_in_newton;
			Refresh();
		}
	}
	_mouse.state = Mouse::unpressed;
};

void p6::Frame::OnRightDown(wxMouseEvent &e)
{

};

void p6::Frame::OnRightUp(wxMouseEvent &e)
{

};

void p6::Frame::OnMove(wxMouseEvent &e)
{
	switch (_mouse.state)
	{
	case Mouse::pressed_on_panel:
	case Mouse::pressed_on_stick:
	case Mouse::pressed_on_force:
			_mouse.state = Mouse::move_on_panel;
			break;

	case Mouse::pressed_on_node:
		_mouse.state = Mouse::move_on_node;
		break;
	}

	if (_mouse.state == Mouse::move_on_node)
	{
		if (_tool == Tool::force)
		{
			real x = _pixel_to_real_x(e.GetX());
			real y = _pixel_to_real_y(e.GetY());
			_task.nodes[_mouse.index].xforce = (x - _task.nodes[_mouse.index].x) / _meters_in_newton;
			_task.nodes[_mouse.index].yforce = (y - _task.nodes[_mouse.index].y) / _meters_in_newton;
			Refresh();
		}
		else
		{
			_task.nodes[_mouse.index].x = _pixel_to_real_x(e.GetX());
			_task.nodes[_mouse.index].y = _pixel_to_real_y(e.GetY());
			Refresh();
		}
	}
	else if (_mouse.state == Mouse::move_on_panel)
	{
		if (_tool == Tool::area)
		{
			_mouse.xup = e.GetX();
			_mouse.yup = e.GetY();
			Refresh();
		}
		else
		{
			_x = _mouse.xold + (_mouse.xdown - e.GetX()) / _pixels_in_meter;
			_y = _mouse.yold + (e.GetY() - _mouse.ydown) / _pixels_in_meter;
			Refresh();
		}
	}
};

void p6::Frame::OnWheel(wxMouseEvent &e)
{
	if (_tool == Tool::force)
	{
		if (e.GetWheelRotation() > 0) _mouse.force_wheel++;
		else if (e.GetWheelRotation() < 0) _mouse.force_wheel--;
		_meters_in_newton = exp2(_mouse.force_wheel);
	}
	else
	{
		if (e.GetWheelRotation() > 0) _mouse.wheel++;
		else if (e.GetWheelRotation() < 0) _mouse.wheel--;
		_pixels_in_meter = 30.0 * exp2(_mouse.wheel);
	}
	Refresh();
};

void p6::Frame::OnPaint(wxPaintEvent &e)
{
	wxPaintDC dc(this);
	_render(&dc, true);
};

void p6::Frame::OnSize(wxSizeEvent &e)
{
	Refresh();
	Layout();
};