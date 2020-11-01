#include "../header/p6_menubar.h"

p6::MenuBar::MenuBar(Frame *frame) : _frame(frame)
{
	wxMenuBar *menubar = new wxMenuBar();

	//File
	wxMenu *filemenu = new wxMenu();
	wxMenuItem *item = filemenu->Append(wxID_ANY, "Load", "Load construction from file");
	frame->Bind(wxEVT_MENU, &OnMenuFileLoad, this, item->GetId());
	item = filemenu->Append(wxID_ANY, "Import", "Import construction from file");
	frame->Bind(wxEVT_MENU, &OnMenuFileImport, this, item->GetId());
	item = filemenu->Append(wxID_ANY, "Save", "Save construction to file");
	frame->Bind(wxEVT_MENU, &OnMenuFileSave, this, item->GetId());
	item = filemenu->Append(wxID_ANY, "Save as", "Save construction another file");
	frame->Bind(wxEVT_MENU, &OnMenuFileSaveAs, this, item->GetId());
	menubar->Append(filemenu, "File");

	//Export
	wxMenu *exportmenu = new wxMenu();
	item = exportmenu->Append(wxID_ANY, "PNG", "Export image as PNG with white background");
	frame->Bind(wxEVT_MENU, &OnMenuExportPNG, this, item->GetId());
	item = exportmenu->Append(wxID_ANY, "JPEG", "Export image as JPEG");
	frame->Bind(wxEVT_MENU, &OnMenuExportJPEG, this, item->GetId());
	menubar->Append(exportmenu, "Export");

	//Help
	wxMenu *helpmenu = new wxMenu();
	item = helpmenu->Append(wxID_ANY, "Help", "Get help");
	frame->Bind(wxEVT_MENU, &OnMenuHelp, this, item->GetId());
	menubar->Append(helpmenu, "Help");

	frame->SetMenuBar(menubar);
};

void p6::MenuBar::OnMenuFileLoad(wxCommandEvent &e)
{
	wxFileDialog dialog(_frame, "Open P6 File", "", "", "P6 File (*.p6)|*.p6", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		try
		{
			_frame->construction.load(dialog.GetPath().ToStdString());
			_file = dialog.GetPath().ToStdString();
			_frame->selection.nodes.clear();
			_frame->selection.sticks.clear();
			_frame->selection.forces.clear();
		}
		catch (std::exception &e)
		{
			wxMessageBox(e.what(), "Error", wxICON_ERROR, _frame);
		}
	}
};

void p6::MenuBar::OnMenuFileImport(wxCommandEvent &e)
{
	wxFileDialog dialog(_frame, "Import P6 File", "", "", "P6 File (*.p6)|*.p6", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		try
		{
			size_t old_node_count = _frame->construction.get_node_count();
			size_t old_stick_count = _frame->construction.get_stick_count();
			size_t old_force_count = _frame->construction.get_force_count();
			_frame->construction.import(dialog.GetPath().ToStdString());
			_frame->selection.nodes.clear();
			for (size_t i = old_node_count; i < _frame->construction.get_node_count(); i++)
			{
				_frame->selection.nodes.insert(i);
			}
			_frame->selection.sticks.clear();
			for (size_t i = old_stick_count; i < _frame->construction.get_stick_count(); i++)
			{
				_frame->selection.sticks.insert(i);
			}
			_frame->selection.forces.clear();
			for (size_t i = old_force_count; i < _frame->construction.get_force_count(); i++)
			{
				_frame->selection.forces.insert(i);
			}
		}
		catch (std::exception &e)
		{
			wxMessageBox(e.what(), "Error", wxICON_ERROR, _frame);
		}
	}
};

void p6::MenuBar::OnMenuFileSave(wxCommandEvent &e)
{
	if (_file == "")
	{
		wxFileDialog dialog(_frame, "Save P6 File", "", "", "P6 File (*.p6)|*.p6", wxFD_SAVE);
		if (dialog.ShowModal() == wxID_CANCEL) return;
		_file = dialog.GetPath();
	}

	try
	{
		_frame->construction.save(_file);
	}
	catch (std::exception &e)
	{
		wxMessageBox(e.what(), "Error", wxICON_ERROR, _frame);
	}
};

void p6::MenuBar::OnMenuFileSaveAs(wxCommandEvent &e)
{
	wxFileDialog dialog(_frame, "Save P6 File as...", "", "", "P6 File (*.p6)|*.p6", wxFD_SAVE);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		try
		{
			_frame->construction.save(dialog.GetPath().ToStdString());
		}
		catch (std::exception &e)
		{
			wxMessageBox(e.what(), "Error", wxICON_ERROR, _frame);
		}
	}
};

void p6::MenuBar::OnMenuExportPNG(wxCommandEvent &e)
{
	wxFileDialog dialog(_frame, "Export as PNG", "", "", "PNG Image (*.png)|*.png", wxFD_SAVE);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		wxMemoryDC dc;
		wxSize size = wxSize(_frame->main_panel.width(), _frame->main_panel.height());
		wxBitmap bitmap(size);
		dc.SelectObject(bitmap);
		dc.SetPen(wxPen(wxColour(255, 255, 255), 0));
		dc.SetBrush(wxBrush(wxColour(255, 255, 255)));
		dc.DrawRectangle(wxPoint(0, 0), size);
		_frame->main_panel.render(&dc);
		bitmap.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_PNG);
	}
};

void p6::MenuBar::OnMenuExportJPEG(wxCommandEvent &e)
{
	wxFileDialog dialog(_frame, "Export as JPEG", "", "", "JPEG Image (*.jpg)|*.jpg", wxFD_SAVE);
	if (dialog.ShowModal() != wxID_CANCEL)
	{
		wxMemoryDC dc;
		wxSize size = wxSize(_frame->main_panel.width(), _frame->main_panel.height());
		wxBitmap bitmap(size);
		dc.SelectObject(bitmap);
		dc.SetPen(wxPen(wxColour(255, 255, 255), 0));
		dc.SetBrush(wxBrush(wxColour(255, 255, 255)));
		dc.DrawRectangle(wxPoint(0, 0), size);
		_frame->main_panel.render(&dc);
		bitmap.SaveFile(dialog.GetPath(), wxBITMAP_TYPE_JPEG);
	}
};

void p6::MenuBar::OnMenuHelp(wxCommandEvent &e)
{
	wxMessageBox("Ask Kyrylo", "Help", wxICON_INFORMATION, _frame);
};