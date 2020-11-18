/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_STICK_BAR
#define P6_STICK_BAR

#include <wx/wx.h>

namespace p6
{
	class Frame;

	///Stick bar is used to read and edit data of selected sticks in text form, one of possible contents of side bar
	class StickBar
	{
	private:
		Frame *_frame;							///<Application's window
		wxStaticText *_material_static;			///<Text explaining material choice box
		wxChoice *_material_choice; 			///<Material choice box
		wxStaticText *_area_static;				///<Text explaining cross-sectional area input box
		wxTextCtrl *_area_text;					///<Cross-sectional area input box
		wxStaticText *_length_static;			///<Text explaining length display box
		wxTextCtrl *_length_text;				///<Length display box
		wxStaticText *_strain_static;			///<Text explaining strain display box
		wxTextCtrl *_strain_text;				///<Strain display box
		wxStaticText *_force_static;			///<Text explaining force display box
		wxTextCtrl *_force_text;				///<Force display box

		void _on_area(wxCommandEvent &e);		///<Handles text input in cross-sectional area input box, changes cross-sectional area of selected sticks
		void _on_material(wxCommandEvent &e);	///<Handles material choice event, changes material of selected sticks

	public:
		StickBar(Frame *frame)		noexcept;	///<Creates stick bar
		void show()					noexcept;	///<Adds bar's components to side bar
		void refresh()				noexcept;	///<Refreshes contents of bar's components, except choice box
		void refresh_materials()	noexcept;	///<Refreshes content of material choice box
		void hide()					noexcept;	///<Removes bar's components from side bar
	};
}

#endif