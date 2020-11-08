/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_MENUBAR
#define P6_MENUBAR

#include "p6_common.hpp"
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
}

#endif