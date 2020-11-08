#include "../header/p6_app.h"
#include "../header/p6_frame.h"

wxIMPLEMENT_APP(p6::App);

bool p6::App::OnInit()
{
	wxImage::AddHandler(new wxPNGHandler);
	_frame = new Frame;
	return true;
}