#ifndef P6_MOUSE_BAR
#define P6_MOUSE_BAR

#include "p6_common.h"
#include <wx/wx.h>

namespace p6
{
	class Frame;

	struct Mouse
	{
	private:
		Frame *_frame;
		int _wheel;						//Used by scaling nodes
		int _wheel_force;				//Used by scaling forces
		real _old_x;					//Used by moving view
		real _old_y;					//Used by moving view
		void _on_left_down(wxMouseEvent &e);
		void _on_left_up(wxMouseEvent &e);
		void _on_right_down(wxMouseEvent &e);
		void _on_right_up(wxMouseEvent &e);
		void _on_move(wxMouseEvent &e);
		void _on_wheel(wxMouseEvent &e);

	public:
		bool moving;					//Used to decide if moving ot clicking
		MainPanel::Item selected_item;	//Used by clicking
		size_t selected_index;			//Used by clicking
		wxPoint point_down;				//Used by dragging and area
		wxPoint point_up;				//Used by dragging and area
		Mouse(Frame *frame);
	};
};

#endif