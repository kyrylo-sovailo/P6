/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_force_bar.hpp"
#include "../header/p6_frame.hpp"

void p6::Frame::_on_paint(wxPaintEvent &e)
{
	wxPaintDC dc(_frame);
	_main_panel.render(&dc, _frame->GetClientAreaOrigin());
}

void p6::Frame::_on_size(wxSizeEvent &e)
{
	_main_panel.need_refresh_image();
	_frame->Layout();
}

p6::Frame::Frame()  noexcept :
	_frame(new wxFrame(nullptr, wxID_ANY, "P6")),
	_sizer(new wxBoxSizer(wxHORIZONTAL)),
	_menubar(this),
	_toolbar(this),
	_main_panel(this),
	_side_panel(this),
	_mouse(this)
{
	_frame->SetSizer(_sizer);
	_frame->Bind(wxEVT_PAINT, &Frame::_on_paint, this, _frame->GetId());
	_frame->Bind(wxEVT_SIZE, &Frame::_on_size, this, _frame->GetId());
	_frame->Show();
}

wxFrame *p6::Frame::frame() noexcept
{
	return _frame;
}

wxBoxSizer *p6::Frame::sizer() noexcept
{
	return _sizer;
}

p6::ToolBar *p6::Frame::toolbar() noexcept
{
	return &_toolbar;
}

p6::MenuBar *p6::Frame::menubar() noexcept
{
	return &_menubar;
}

p6::MainPanel *p6::Frame::main_panel() noexcept
{
	return &_main_panel;
}

p6::SidePanel *p6::Frame::side_panel() noexcept
{
	return &_side_panel;
}

p6::Construction *p6::Frame::construction() noexcept
{
	return &_construction;
}