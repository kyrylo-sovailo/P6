#include "../header/p6_frame.h"

void p6::Frame::_on_paint(wxPaintEvent &e)
{
	wxPaintDC dc(frame);
	main_panel.render(&dc, main_panel.offset());
};

void p6::Frame::_on_size(wxSizeEvent &e)
{
	main_panel.refresh();
	frame->Layout();
};

p6::Frame::Frame() :
	frame(new wxFrame(nullptr, wxID_ANY, "P6")),
	sizer(new wxBoxSizer(wxHORIZONTAL)),
	menubar(this),
	toolbar(this),
	main_panel(this),
	side_panel(this),
	mouse(this)
{
	frame->Bind(wxEVT_PAINT, &Frame::_on_paint, this, frame->GetId());
	frame->Bind(wxEVT_SIZE, &Frame::_on_size, this, frame->GetId());
};