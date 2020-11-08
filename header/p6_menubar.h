#ifndef P6_MENUBAR
#define P6_MENUBAR

#include "p6_common.h"
#include <wx/wx.h>

namespace p6
{
	class Frame;

	class MenuBar
	{
	private:
		Frame *_frame;
		wxMenuBar *_menu;
		String _file;
		void _on_menu_file_load(wxCommandEvent &e);
		void _on_menu_file_import(wxCommandEvent &e);
		void _on_menu_file_save(wxCommandEvent &e);
		void _on_menu_file_save_as(wxCommandEvent &e);
		void _on_menu_export_png(wxCommandEvent &e);
		void _on_menu_export_jpeg(wxCommandEvent &e);
		void _on_menu_help(wxCommandEvent &e);

	public:
		MenuBar(Frame *frame);
	};
};

#endif