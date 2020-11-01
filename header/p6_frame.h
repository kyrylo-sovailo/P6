#ifndef P6_FRAME
#define P6_FRAME

#include "../header/p6_construction.h"
#include "../header/p6_menubar.h"
#include "../header/p6_toolbar.h"
#include "../header/p6_main_panel.h"
#include "../header/p6_side_panel.h"
#include "../header/p6_material_bar.h"
#include <wx/wx.h>

namespace p6
{
	class Frame : public wxFrame
	{
	private:
		void OnPaint(wxPaintEvent &e);
		void OnSize(wxSizeEvent &e);

	public:
		struct Selection
		{
			std::set<unsigned int> nodes;
			std::set<unsigned int> sticks;
			std::set<unsigned int> forces;
		}; 
		
		MenuBar menubar;
		ToolBar toolbar;
		MainPanel main_panel;
		SidePanel side_panel;
		MaterialBar material_bar;
		Construction construction;
		Selection selection;

		Frame();
	};
};

#endif