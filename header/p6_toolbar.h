#ifndef P6_TOOLBAR
#define P6_TOOLBAR

#include <wx/wx.h>

namespace p6
{
	class ToolBar
	{
	private:
		wxToolBar *_toolbar;
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
		wxToolBarToolBase *_simulate;
		wxToolBarToolBase *_select;
		wxToolBarToolBase *_area;
		wxToolBarToolBase *_move;
		wxToolBarToolBase *_node;
		wxToolBarToolBase *_stick;
		wxToolBarToolBase *_force;
		wxToolBarToolBase *_delete;
		void OnToolSimulate(wxCommandEvent &e);
		void OnToolSelect(wxCommandEvent &e);
		void OnToolArea(wxCommandEvent &e);
		void OnToolMove(wxCommandEvent &e);
		void OnToolNode(wxCommandEvent &e);
		void OnToolStick(wxCommandEvent &e);
		void OnToolForce(wxCommandEvent &e);
		void OnToolDelete(wxCommandEvent &e);

	public:
		ToolBar();
	};
};

#endif