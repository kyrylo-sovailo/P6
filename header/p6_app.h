#ifndef P6_APP
#define P6_APP

#include <wx/wx.h>
#include "p6_frame.h"

namespace p6
{
	class App : public wxApp
	{
	private:
		Frame *_frame;

	public:
		virtual bool OnInit();
	};
};

#endif