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

	///Tool bar is used to select tool
	class ToolBar
	{
	public:
		///List of available tools
		enum class Tool
		{
			no,
			select,
			area,
			move,
			node,
			stick,
			force,
			delet
		};

	private:
		Frame *_frame;							///<Application's window
		wxToolBar *_toolbar;					///<wxWidget's tool bar
		wxToolBarToolBase *_simulate;			///<Simulation button, switches between editing and simulation
		wxToolBarToolBase *_select;				///<Selection tool, select items
		wxToolBarToolBase *_area;				///<Area selection tool, select items in area
		wxToolBarToolBase *_move;				///<Move tool, moves multiply items at one time
		wxToolBarToolBase *_node;				///<Node tool, creates nodes or selects nodes only
		wxToolBarToolBase *_stick;				///<Stick tool, creates sticks or selects sticks only
		wxToolBarToolBase *_force;				///<Force tool, creates forces or selects forces only
		wxToolBarToolBase *_delete;				///<Deletion tool, deletes items
		Tool _tool = Tool::no;					///<Currently selected tool
		bool _simulation = false;				///<Indicator if simulation is being run

		void _on_simulate(wxCommandEvent &e);	///<Handles press on simulation button, switches between editing and simulation
		void _on_select(wxCommandEvent &e);		///<Handles press on selection tool, turns selection tool on or off
		void _on_area(wxCommandEvent &e);		///<Handles press on area selection tool, turns area selection tool on or off
		void _on_move(wxCommandEvent &e);		///<Handles press on move tool, turns move tool on or off
		void _on_node(wxCommandEvent &e);		///<Handles press on node tool, turns node tool on or off, or selects nodes only
		void _on_stick(wxCommandEvent &e);		///<Handles press on stick tool, turns stick tool on or off, or selects nodes only, or creates stick between two selected nodes
		void _on_force(wxCommandEvent &e);		///<Handles press on node tool, turns node tool on or off, or selects forces only
		void _on_delete(wxCommandEvent &e);		///<Handles press on deletion tool, turns deletion tool on or off, or deletes selected items
		void _refresh() noexcept;				///<Refresh states and activeness of buttons
		wxBitmap _load_png(const String filepath) noexcept;	///<Loads bitmap from PNG file

	public:
		ToolBar(Frame *frame)	noexcept;		///<Creates tool bar
		Tool tool()				const noexcept;	///<Returns selected tool
		bool simulation()		const noexcept;	///<Returns if construction is being simulated
	};
}

#endif
