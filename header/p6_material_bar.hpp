/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_MATERIAL_BAR
#define P6_MATERIAL_BAR

#include <wx/wx.h>

namespace p6
{
	class Frame;

	///Material bar is used to read and edit materials and their characteristics
	class MaterialBar
	{
	private:
		Frame *_frame;							///<Application's window
		wxChoice *_material_choice;				///<Choice of material
		wxStaticText *_name_static;				///<Text explaining material name edit box
		wxTextCtrl *_name_text;					///<Material name edit box
		wxCheckBox *_nonlinear_check;			///<Check box deciding if material is nonlinear
		wxStaticText *_formula_static;			///<Text explaining material's formula edit box
		wxTextCtrl *_formula_text;				///<Edit box containing material's stress(strain) formula or Young's modulus
		wxBoxSizer *_buttons_sizer;				///<Sizer for three buttons
		wxButton *_button_new;					///<Button for creating new material
		wxButton *_button_apply;				///<Button for applying changes to selected material
		wxButton *_button_delete;				///<Button for deleting selected material

		void _on_choice(wxCommandEvent &e);		///<Handles material choice event, displays material's data
		void _on_new(wxCommandEvent &e);		///<Handles press of "new" button, clears material bar and deselects material
		void _on_apply(wxCommandEvent &e);		///<Handles press of "apply" button, saves material with given characteristics
		void _on_delete(wxCommandEvent &e);		///<Handles press of "delete" button, deletes selected material

	public:
		MaterialBar(Frame *frame)	noexcept;	///<Creates material bar
		void show()					noexcept;	///<Adds bar's components to side bar
		void refresh()				noexcept;	///<Refreshes contents of bar's components, except choice box
		void refresh_materials()	noexcept;	///<Refreshes content of material choice box
		void hide()					noexcept;	///<Removes bar's components from side bar
	};
}

#endif