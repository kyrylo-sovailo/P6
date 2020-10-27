#ifndef P6_FRAME
#define P6_FRAME

#include <wx/wx.h>
#include <set>
#include "p6_construction.h"

namespace p6
{
	class Frame : public wxFrame
	{
	public:
		Frame();
		~Frame();

	private:
		
		//Selected tool
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
		} _tool = Tool::no;

		//Bars
		struct
		{
			wxToolBarToolBase *simulate_tool; 
			wxToolBarToolBase *select_tool;
			wxToolBarToolBase *area_tool;
			wxToolBarToolBase *move_tool;
			wxToolBarToolBase *node_tool;
			wxToolBarToolBase *stick_tool;
			wxToolBarToolBase *force_tool;
			wxToolBarToolBase *delete_tool;
		} _toolbar;

		struct
		{
			wxBoxSizer *sizer;
			wxCheckBox *free_check;
			wxCheckBox *fixed_check;
			wxTextCtrl *x_edit;
			wxTextCtrl *y_edit;
		} _node_bar;
		
		struct
		{
			wxBoxSizer *sizer;
			wxTextCtrl *area_edit;
			wxChoice *material_choice;
			wxStaticText *length_text;
			wxStaticText *strain_text;
			wxStaticText *force_text;
		} _stick_bar;
		
		struct
		{
			wxBoxSizer *sizer;
			wxTextCtrl *x_edit;
			wxTextCtrl *y_edit;
		} _force_bar;
		
		struct
		{
			wxBoxSizer *sizer;
			wxChoice *name_choice;
			wxTextCtrl *name_edit;
			wxButton *new_button;
			wxButton *apply_button;
			wxButton *delete_button;
			wxCheckBox *nonlinear_check;
			wxTextCtrl *formula_edit;
		} _material_bar;
		
		struct
		{
			wxBoxSizer *sizer;
			wxTextCtrl *anchor_x_edit;
			wxTextCtrl *anchor_y_edit;
			wxTextCtrl *scale_edit;
			wxTextCtrl *angle_edit;
			wxTextCtrl *shift_x_edit;
			wxTextCtrl *shift_y_edit;
		} _move_bar;
		
		//Selection
		struct
		{
			std::set<unsigned int> nodes;
			std::set<unsigned int> sticks;
			std::set<unsigned int> forces;
		} _selection;

		//Mouse
		enum class Mouse
		{
			unpressed,
			pressed_on_panel,
			pressed_on_node,
			pressed_on_stick,
			pressed_on_force,
			move_on_panel,
			move_on_node
		};
		struct
		{
			Mouse state;
			unsigned int index;
			real xold;			//Old _x, used for dragging
			real yold;			//Old _y, used for dragging
			wxCoord xdown;
			wxCoord ydown;
			wxCoord xup;		//Current e.GetX(), used for area
			wxCoord yup;		//Current e.GetY(), used for area
			int wheel;			//Used for scaling
			int force_wheel;	//Used for scaling forces
		} _mouse;

		//Main elements
		wxPanel *_main_panel;
		wxPanel *_side_panel;
		wxBoxSizer *_sizer;

		//Coordinates
		real _pixels_in_meter = 30;
		real _meters_in_newton = 1;
		real _x = 0.0;
		real _y = 0.0;

		//Move
		struct
		{
			real xanchor = 0.0;
			real yanchor = 0.0;
			real scale = 1.0;
			real angle = 0.0;
			real xshift = 0.0;
			real yshift = 0.0;
		} _move;

		//Stored constructions
		Construction _task, _result;

		//Pile path
		String _file_path;

		//Maximal stick strain
		real _max_strain;
		
		//Functions
		void _init_menubar();
		void _init_toolbar();
		void _init_node_bar();
		void _init_stick_bar();
		void _init_force_bar();
		void _init_material_bar();
		void _init_move_bar();
		void _refresh_side_panel();
		void _refresh_toolbar();
		void _refresh_materials();
		void _render(wxDC *dc, bool panel); 
		real _pixel_to_real_x(wxCoord x)				const;
		real _pixel_to_real_y(wxCoord y)				const;
		wxCoord _real_to_pixel_x(real x, bool panel)	const;
		wxCoord _real_to_pixel_y(real y, bool panel)	const;
		real _stick_length(bool sim, unsigned int i)	const;
		real _stick_strain(unsigned int i)				const;
		real _stick_force(unsigned int i)				const;
		void _node_delete(unsigned int i);

		//Menu handlers
		void OnMenuFileLoad(wxCommandEvent &e);
		void OnMenuFileImport(wxCommandEvent &e); 
		void OnMenuFileSave(wxCommandEvent &e);
		void OnMenuFileSaveAs(wxCommandEvent &e);
		void OnMenuExportPNG(wxCommandEvent &e);
		void OnMenuExportJPEG(wxCommandEvent &e);
		void OnMenuHelp(wxCommandEvent &e);

		//Tool handlers
		void OnToolSimulate(wxCommandEvent &e);
		void OnToolSelect(wxCommandEvent &e);
		void OnToolArea(wxCommandEvent &e);
		void OnToolMove(wxCommandEvent &e);
		void OnToolNode(wxCommandEvent &e);
		void OnToolStick(wxCommandEvent &e);
		void OnToolForce(wxCommandEvent &e);
		void OnToolDelete(wxCommandEvent &e);

		//Node handlers
		void OnNodeFree(wxCommandEvent &e);
		void OnNodeFixed(wxCommandEvent &e);
		void OnNodeX(wxCommandEvent &e);
		void OnNodeY(wxCommandEvent &e);

		//Stick handlers
		void OnStickArea(wxCommandEvent &e);
		void OnStickMaterial(wxCommandEvent &e);

		//Force handlers
		void OnForceX(wxCommandEvent &e);
		void OnForceY(wxCommandEvent &e);

		//Material handlers
		void OnMaterialChoice(wxCommandEvent &e);
		void OnMaterialName(wxCommandEvent &e);
		void OnMaterialNew(wxCommandEvent &e);
		void OnMaterialApply(wxCommandEvent &e);
		void OnMaterialDelete(wxCommandEvent &e);
		void OnMaterialNonlinear(wxCommandEvent &e);
		void OnMaterialFormula(wxCommandEvent &e);

		//Move handlers
		void OnMoveAnchorX(wxCommandEvent &e);
		void OnMoveAnchorY(wxCommandEvent &e);
		void OnMoveScale(wxCommandEvent &e);
		void OnMoveAngle(wxCommandEvent &e);
		void OnMoveShiftX(wxCommandEvent &e);
		void OnMoveShiftY(wxCommandEvent &e);

		//Mouse handlers
		void OnLeftDown(wxMouseEvent &e);
		void OnLeftUp(wxMouseEvent &e);
		void OnRightDown(wxMouseEvent &e);
		void OnRightUp(wxMouseEvent &e);
		void OnMove(wxMouseEvent &e);
		void OnWheel(wxMouseEvent &e);

		//Paint handler
		void OnPaint(wxPaintEvent &e);

		//Size handler
		void OnSize(wxSizeEvent &e);
	};
};

#endif