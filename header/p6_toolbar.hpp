/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_TOOLBAR
#define P6_TOOLBAR

#include "p6_common.hpp"
#include <wx/wx.h>

namespace p6
{
	class Frame;

	class ToolBar
	{
	public:
		enum class Tool
		{
			no,
			simulate,
			select,
			area,
			move,
			node,
			stick,
			force,
			delet
		};

	private:
		Frame *_frame;
		wxToolBar *_toolbar;
		wxToolBarToolBase *_simulate;
		wxToolBarToolBase *_select;
		wxToolBarToolBase *_area;
		wxToolBarToolBase *_move;
		wxToolBarToolBase *_node;
		wxToolBarToolBase *_stick;
		wxToolBarToolBase *_force;
		wxToolBarToolBase *_delete;
		Tool _tool = Tool::no;

		void _on_simulate(wxCommandEvent &e);
		void _on_select(wxCommandEvent &e);
		void _on_area(wxCommandEvent &e);
		void _on_move(wxCommandEvent &e);
		void _on_node(wxCommandEvent &e);
		void _on_stick(wxCommandEvent &e);
		void _on_force(wxCommandEvent &e);
		void _on_delete(wxCommandEvent &e);
		static wxBitmap _load_png(const String filepath);

	public:
		ToolBar(Frame *frame);
		Tool tool() const;
	};
}

#endif