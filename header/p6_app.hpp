/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_APP
#define P6_APP

#include "p6_frame.hpp"
#include <wx/wx.h>

namespace p6
{
	///App refresents application
	class App : public wxApp
	{
	private:
		Frame *_frame;			///<Pointer to application's window

		virtual bool OnInit();	///< Handler that is called after application's creations. Creates p6::Frame
	};
}

#endif