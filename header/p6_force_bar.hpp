/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_FORCE_BAR
#define P6_FORCE_BAR

#include <wx/wx.h>

namespace p6
{
	class Frame;
	
	///Force bar is used to read and edit data of selected forces in text form, one of possible contents of side bar
	class ForceBar
	{
	private:
		Frame *_frame;						///<Application's window
		wxStaticText *_x_static;			///<Text explaining horizontal force edit box
		wxTextCtrl *_x_text;				///<Horizontal force edit box
		wxStaticText *_y_static;			///<Text explaining vertical force edit box
		wxTextCtrl *_y_text;				///<Vertical force edit box
		wxStaticText *_modulus_static;		///<Text explaining force modulus edit box
		wxTextCtrl *_modulus_text;			///<Force modulus edit box
		wxStaticText *_angle_static;		///<Text explaining force angle edit box
		wxTextCtrl *_angle_text;			///<Force angle (in grad) edit box
		
		void _on_x(wxCommandEvent &e);		///<Handles text input in horizontal force edit box, sets x component of all selected forces
		void _on_y(wxCommandEvent &e);		///<Handles text input in vertical force edit box, sets y component of all selected forces
		void _on_modulus(wxCommandEvent &e);///<Handles text input in force modulus edit box, sets modulus of all selected forces
		void _on_angle(wxCommandEvent &e);	///<Handles text input in force angle edit box, sets angle of all selected forces
	
	public:
		ForceBar(Frame *frame)	noexcept;	///<Creates force bar
		void show()				noexcept;	///<Adds bar's components to side bar
		void refresh()			noexcept;	///<Refreshes contents of bar's components
		void hide()				noexcept;	///<Removes bar's components from side bar
	};
}

#endif