/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_menubar.hpp"
#include "../header/p6_frame.hpp"

void p6::MenuBar::_on_file_load(wxCommandEvent &e)
{
	wxFileDialog dialog(_frame->frame(), "Open P6 File", "", "", "P6 File (*.p6)|*.p6", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		try
		{
			_frame->construction()->load(dialog.GetPath().ToStdString());
			_file = dialog.GetPath().ToStdString();
			_frame->main_panel()->selected_nodes.clear();
			_frame->main_panel()->selected_sticks.clear();
			_frame->main_panel()->selected_forces.clear();
			_frame->main_panel()->need_refresh();
			_frame->side_panel()->refresh();
			_frame->side_panel()->refresh_materials();
		}
		catch (std::exception &e)
		{
			wxMessageBox(e.what(), "Error", wxICON_ERROR, _frame->frame());
		}
	}
}

void p6::MenuBar::_on_file_import(wxCommandEvent &e)
{
	wxFileDialog dialog(_frame->frame(), "Import P6 File", "", "", "P6 File (*.p6)|*.p6", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		try
		{
			uint old_node_count = _frame->construction()->get_node_count();
			uint old_stick_count = _frame->construction()->get_stick_count();
			uint old_force_count = _frame->construction()->get_force_count();
			_frame->construction()->import(dialog.GetPath().ToStdString());
			_frame->main_panel()->selected_nodes.clear();
			for (uint i = old_node_count; i < _frame->construction()->get_node_count(); i++)
			{
				_frame->main_panel()->selected_nodes.insert(i);
			}
			_frame->main_panel()->selected_sticks.clear();
			for (uint i = old_stick_count; i < _frame->construction()->get_stick_count(); i++)
			{
				_frame->main_panel()->selected_sticks.insert(i);
			}
			_frame->main_panel()->selected_forces.clear();
			for (uint i = old_force_count; i < _frame->construction()->get_force_count(); i++)
			{
				_frame->main_panel()->selected_forces.insert(i);
			}
			_frame->main_panel()->need_refresh();
			_frame->side_panel()->refresh();
			_frame->side_panel()->refresh_materials();
		}
		catch (std::exception &e)
		{
			wxMessageBox(e.what(), "Error", wxICON_ERROR, _frame->frame());
		}
	}
}

void p6::MenuBar::_on_file_save(wxCommandEvent &e)
{
	if (_file == "")
	{
		wxFileDialog dialog(_frame->frame(), "Save P6 File", "", "", "P6 File (*.p6)|*.p6", wxFD_SAVE);
		if (dialog.ShowModal() == wxID_CANCEL) return;
		_file = dialog.GetPath();
	}

	try
	{
		_frame->construction()->save(_file);
	}
	catch (std::exception &e)
	{
		wxMessageBox(e.what(), "Error", wxICON_ERROR, _frame->frame());
	}
}

void p6::MenuBar::_on_file_save_as(wxCommandEvent &e)
{
	wxFileDialog dialog(_frame->frame(), "Save P6 File as...", "", "", "P6 File (*.p6)|*.p6", wxFD_SAVE);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		try
		{
			_frame->construction()->save(dialog.GetPath().ToStdString());
		}
		catch (std::exception &e)
		{
			wxMessageBox(e.what(), "Error", wxICON_ERROR, _frame->frame());
		}
	}
}

void p6::MenuBar::_on_export_png(wxCommandEvent &e)
{
	wxFileDialog dialog(_frame->frame(), "Export as PNG", "", "", "PNG Image (*.png)|*.png", wxFD_SAVE);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		wxMemoryDC dc;
		wxSize size = _frame->main_panel()->size();
		wxBitmap bitmap(size);
		dc.SelectObject(bitmap);
		dc.SetPen(wxPen(wxColour(255, 255, 255), 0));
		dc.SetBrush(wxBrush(wxColour(255, 255, 255)));
		dc.DrawRectangle(wxPoint(0, 0), size);
		_frame->main_panel()->render(&dc, wxPoint(0, 0));
		bitmap.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_PNG);
	}
}

void p6::MenuBar::_on_export_jpeg(wxCommandEvent &e)
{
	wxFileDialog dialog(_frame->frame(), "Export as JPEG", "", "", "JPEG Image (*.jpg)|*.jpg", wxFD_SAVE);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		wxMemoryDC dc;
		wxSize size = _frame->main_panel()->size();
		wxBitmap bitmap(size);
		dc.SelectObject(bitmap);
		dc.SetPen(wxPen(wxColour(255, 255, 255), 0));
		dc.SetBrush(wxBrush(wxColour(255, 255, 255)));
		dc.DrawRectangle(wxPoint(0, 0), size);
		_frame->main_panel()->render(&dc, wxPoint(0, 0));
		bitmap.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_JPEG);
	}
}

void p6::MenuBar::_on_help(wxCommandEvent &e)
{
	wxMessageBox("The developer highly recommends praying to Pinkie Pie to make it work", "Help", wxICON_INFORMATION, _frame->frame());
}

p6::MenuBar::MenuBar(Frame *frame) noexcept : _frame(frame)
{
	wxMenuBar *menubar = new wxMenuBar();

	//File
	wxMenu *filemenu = new wxMenu();
	_file_load = filemenu->Append(wxID_ANY, "Load", "Load construction from file");
	frame->frame()->Bind(wxEVT_MENU, &MenuBar::_on_file_load, this, _file_load->GetId());
	_file_import = filemenu->Append(wxID_ANY, "Import", "Import construction from file");
	frame->frame()->Bind(wxEVT_MENU, &MenuBar::_on_file_import, this, _file_import->GetId());
	wxMenuItem *item = filemenu->Append(wxID_ANY, "Save", "Save construction to file");
	frame->frame()->Bind(wxEVT_MENU, &MenuBar::_on_file_save, this, item->GetId());
	item = filemenu->Append(wxID_ANY, "Save as", "Save construction another file");
	frame->frame()->Bind(wxEVT_MENU, &MenuBar::_on_file_save_as, this, item->GetId());
	menubar->Append(filemenu, "File");

	//Export
	wxMenu *exportmenu = new wxMenu();
	item = exportmenu->Append(wxID_ANY, "PNG", "Export image as PNG with white background");
	frame->frame()->Bind(wxEVT_MENU, &MenuBar::_on_export_png, this, item->GetId());
	item = exportmenu->Append(wxID_ANY, "JPEG", "Export image as JPEG");
	frame->frame()->Bind(wxEVT_MENU, &MenuBar::_on_export_jpeg, this, item->GetId());
	menubar->Append(exportmenu, "Export");

	//Help
	wxMenu *helpmenu = new wxMenu();
	item = helpmenu->Append(wxID_ANY, "Help", "Get help");
	frame->frame()->Bind(wxEVT_MENU, &MenuBar::_on_help, this, item->GetId());
	menubar->Append(helpmenu, "Help");

	frame->frame()->SetMenuBar(menubar);
}

void p6::MenuBar::refresh() noexcept
{
	bool sim = _frame->toolbar()->simulation();
	_file_load->Enable(!sim);
	_file_import->Enable(!sim);
}