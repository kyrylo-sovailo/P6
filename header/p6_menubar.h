#ifndef P6_MENUBAR
#define P6_MENUBAR

#include "p6_frame.h"
#include <wx/wx.h>

namespace p6
{
	class MenuBar
	{
	private:
		Frame *_frame;
		wxMenuBar *_menu;
		String _file;
		void OnMenuFileLoad(wxCommandEvent &e);
		void OnMenuFileImport(wxCommandEvent &e);
		void OnMenuFileSave(wxCommandEvent &e);
		void OnMenuFileSaveAs(wxCommandEvent &e);
		void OnMenuExportPNG(wxCommandEvent &e);
		void OnMenuExportJPEG(wxCommandEvent &e);
		void OnMenuHelp(wxCommandEvent &e);

	public:
		MenuBar(Frame *frame);
	};
};

#endif