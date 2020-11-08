#ifndef P6_SIDE_PANEL
#define P6_SIDE_PANEL

#include "p6_node_bar.h"
#include "p6_stick_bar.h"
#include "p6_force_bar.h"
#include "p6_material_bar.h"
#include "p6_move_bar.h"
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

		Frame *_frame;
		Mode _mode = Mode::material;
		NodeBar _node_bar;
		StickBar _stick_bar;
		ForceBar _force_bar;
		MaterialBar _material_bar;
		MoveBar _move_bar;
		void _switch(Mode mode);

	public:
		wxPanel *panel;
		wxBoxSizer *sizer;
		SidePanel(Frame *frame);
		void refresh();
	};
};

#endif