#ifndef P6_SIDE_PANEL
#define P6_SIDE_PANEL

#include "../header/p6_material_bar.h"
#include <wx/wx.h>

namespace p6
{
	class SidePanel
	{
	private:
		enum class Mode
		{
			node,
			stick,
			force,
			move,
			material
		};

		wxPanel *_panel;
		wxBoxSizer *_sizer;
		Mode _mode = Mode::material;

		friend MaterialBar;
	};
};

#endif