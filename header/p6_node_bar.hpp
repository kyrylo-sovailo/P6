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
		wxCheckBox *_fixed_check;			///<Check box deciding if all nodes are fixed
		wxCheckBox *_free1d_check;			///<Check box deciding if all nodes are fixed on rail
		wxCheckBox *_free2d_check;			///<Check box deciding if all nodes are free
		wxStaticText *_x_static;			///<Text explaining nodes' X coordinate edit box
		wxTextCtrl *_x_text;				///<Nodes' X coordinate edit box
		wxStaticText *_y_static;			///<Text explaining nodes' Y coordinate edit box
		wxTextCtrl *_y_text;				///<Nodes' Y coordinate edit box
		wxStaticText *_angle_static;		///<Text explaining nodes' rail angle edit box for nodes fixed on rail
		wxTextCtrl *_angle_text;			///<Nodes' Rail angle edit box for nodes fixed on rail

		void _on_fixed(wxCommandEvent &e);	///<Handles checking "fixed" check box, fixes all nodes
		void _on_free1d(wxCommandEvent &e);	///<Handles checking "fixed on rail" check box, fixes all nodes on rail
		void _on_free2d(wxCommandEvent &e);	///<Handles checking "free" check box, frees all nodes
		void _on_x(wxCommandEvent &e);		///<Handles text input in nodes' X coordinate edit box, sets X coordinate of all selected nodes
		void _on_y(wxCommandEvent &e);		///<Handles text input in nodes' Y coordinate edit box, sets Y coordinate of all selected nodes
		void _on_angle(wxCommandEvent &e);	///<Handles text input in nodes' rail angle edit box, sets rail angle of selected nodes fixed on rail

	public:
		NodeBar(Frame *frame)	noexcept;	///<Creates node bar
		void show()				noexcept;	///<Adds bar's components to side bar
		void refresh()			noexcept;	///<Refreshes contents of bar's components
		void hide()				noexcept;	///<Removes bar's components from side bar
	};
}

#endif