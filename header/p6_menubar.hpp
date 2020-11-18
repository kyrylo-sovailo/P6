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

	///Window's menu bar is used mostly for file operations
	class MenuBar
	{
	private:
		Frame *_frame;								///<Application's window				
		wxMenuBar *_menu;							///<wxWidgets's menu
		wxMenuItem *_file_load;						///<wxWidgets's "Load file" menu item
		wxMenuItem *_file_import;					///<wxWidgets's "Import file" menu item
		String _file;								///<Current file's path

		void _on_file_load(wxCommandEvent &e);		///<Handles press on "file load" menu item, loads construction
		void _on_file_import(wxCommandEvent &e);	///<Handles press on "file import" menu item, imports construction
		void _on_file_save(wxCommandEvent &e);		///<Handles press on "file save" menu item, asks file name if unknown and saves construction
		void _on_file_save_as(wxCommandEvent &e);	///<Handles press on "file save as" menu item, asks file name and saves construction to it
		void _on_export_png(wxCommandEvent &e);		///<Handles press on "export PNG" menu item, saves construction image in PNG format
		void _on_export_jpeg(wxCommandEvent &e);	///<Handles press on "export JPEG" menu item, saves construction image in JPEG format
		void _on_help(wxCommandEvent &e);			///<Handles press on "help" menu item, displays help message

	public:
		MenuBar(Frame *frame)	noexcept;			///<Creates menu bar
		void refresh() noexcept;			///<Refreshes activeness of menu items
	};
}

#endif