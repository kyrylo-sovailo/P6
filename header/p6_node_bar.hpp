/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_NODE_BAR
#define P6_NODE_BAR

#include <wx/wx.h>

namespace p6
{
	class Frame;

	///Node bar is used to read and edit data of selected nodes in text form, one of possible contents of side bar
	class NodeBar
	{
	private:
		Frame *_frame;						///<Application's window
		wxCheckBox *_free_check;			///<Check box deciding if all nodes are free
		wxCheckBox *_fixed_check;			///<Check box deciding if all nodes are fixed
		wxStaticText *_x_static;			///<Text explaining nodes' X coordinate edit box
		wxTextCtrl *_x_text;				///<Nodes' X coordinate edit box
		wxStaticText *_y_static;			///<Text explaining nodes' Y coordinate edit box
		wxTextCtrl *_y_text;				///<Nodes' Y coordinate edit box

		void _on_free(wxCommandEvent &e);	///<Handles checking "free" check box, frees all nodes
		void _on_fixed(wxCommandEvent &e);	///<Handles checking "fixed" check box, fixes all nodes
		void _on_x(wxCommandEvent &e);		///<Handles text input in node's X coordinate edit box, sets X coordinate of all selected nodes
		void _on_y(wxCommandEvent &e);		///<Handles text input in node's Y coordinate edit box, sets Y coordinate of all selected nodes

	public:
		NodeBar(Frame *frame)	noexcept;	///<Creates node bar
		void show()				noexcept;	///<Adds bar's components to side bar
		void refresh_controls()	noexcept;	///<Refreshes contents and activeness of bar's components
		void hide()				noexcept;	///<Removes bar's components from side bar
	};
}

#endif