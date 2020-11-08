/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_frame.hpp"
#include "../header/p6_utils.hpp"

wxPoint p6::MainPanel::_real_to_pixel(real x, real y, wxPoint offset) const
{
	return offset + wxPoint(
		panel->GetSize().x / 2 + (x - center_x) * pixels_in_meter,
		panel->GetSize().y / 2 - (y - center_y) * pixels_in_meter);
}

p6::MainPanel::MainPanel(Frame *frame)
{
	_frame = frame;
	panel = new wxPanel(frame->frame);
	panel->Show(false);
	frame->sizer->Add(panel, 4, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN, 0);
}

wxSize p6::MainPanel::size() const
{
	return panel->GetSize();
}

wxPoint p6::MainPanel::offset() const
{
	return panel->GetClientAreaOrigin();
}

void p6::MainPanel::render(wxDC *dc, wxPoint offset) const
{
	bool sim = _frame->construction.simulation();
	Construction *con = &_frame->construction;

	//Calculate max strain
	real max_strain = 0.0;
	for (uint i = 0; i < con->get_stick_count(); i++)
	{
		if (con->get_stick_strain(i) > max_strain) max_strain = con->get_stick_strain(i);
	}

	//Draw sticks
	for (uint i = 0; i < _frame->construction.get_stick_count(); i++)
	{
		uint node_index[2];
		_frame->construction.get_stick_node(i, node_index);

		wxColour colour;
		if (sim)
		{
			real strain = con->get_stick_strain(i) / max_strain;

			if (strain > 0)
			{
				real rg = std::max(128.0 - 128.0 * strain, 0.0);
				real b = std::min(128.0 + 127 * strain, 255.0);
				colour = wxColour(rg, rg, b);
			}
			else
			{
				//Red
				real gb = std::max(128.0 + 128 * strain, 0.0);
				real r = std::min(128.0 - 127.0 * strain, 255.0);
				colour = wxColour(r, gb, gb);
			}
		}
		else colour = wxColour(128, 128, 128);

		wxPoint points[2];
		for (uint j = 0; j < 2; j++)
		{
			real x = con->get_node_x(node_index[j]);
			real y = con->get_node_y(node_index[j]); 
			points[j] = _real_to_pixel(x, y, offset);
		}
		if (selected_sticks.count(i))
		{
			dc->SetPen(wxPen(wxColour(255, 255, 0), 4));
			dc->DrawLine(points[0], points[1]);
		}
		dc->SetPen(wxPen(colour, 3));
		dc->DrawLine(points[0], points[1]);
	}

	//Draw forces
	for (uint i = 0; i < con->get_force_count(); i++)
	{
		const real a = 30.0 * (M_PI / 180.0);
		const real c = 0.1;

		real ax = con->get_node_x(con->get_force_node(i));
		real ay = con->get_node_y(con->get_force_node(i));
		real bax = con->get_force_x(i) * meters_in_newton;
		real bay = con->get_force_y(i) * meters_in_newton;
		real bcx = c * (cos(a) * bax - sin(a) * bay);
		real bcy = c * (sin(a) * bax + cos(a) * bay);
		real bdx = c * (cos(a) * bax + sin(a) * bay);
		real bdy = c * (-sin(a) * bax + cos(a) * bay);

		wxPoint pa = _real_to_pixel(ax, ay, offset);
		wxPoint pb = _real_to_pixel(ax - bax, ay - bay, offset);
		wxPoint pc = _real_to_pixel(ax - bax + bcx, ay - bay + bcy, offset);
		wxPoint pd = _real_to_pixel(ax - bax + bdx, ay - bay + bdy, offset);
		if (selected_forces.count(i))
		{
			dc->SetPen(wxPen(wxColour(255, 255, 0), 2));
			dc->DrawLine(pa, pb);
			dc->DrawLine(pb, pc);
			dc->DrawLine(pb, pd);
		}
		dc->SetPen(wxPen(wxColour(0, 255, 0), 1));
		dc->DrawLine(pa, pb);
		dc->DrawLine(pb, pc);
		dc->DrawLine(pb, pd);
	}

	//Draw points
	for (uint i = 0; i < con->get_node_count(); i++)
	{
		wxPoint point = _real_to_pixel(con->get_node_x(i), con->get_node_y(i), offset);
		if (selected_nodes.count(i)) dc->SetPen(wxPen(wxColour(255, 255, 0), 1));
		else dc->SetPen(wxPen(wxColour(0, 0, 0), 1));
		if (con->get_node_free(i)) dc->SetBrush(wxBrush(wxColour(128, 128, 128)));
		else dc->SetBrush(wxBrush(wxColour(64, 64, 64)));
		dc->DrawCircle(point, 5);
	}

	//Draw selection rect
	Mouse *mouse = &_frame->mouse;
	if (_frame->toolbar.tool == ToolBar::Tool::area
		&& mouse->moving
		&& mouse->selected_item != Item::node)
	{
		dc->SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SHORT_DASH));
		dc->DrawLine(mouse->point_down, wxPoint(mouse->point_up.x, mouse->point_down.y));
		dc->DrawLine(mouse->point_up, wxPoint(mouse->point_up.x, mouse->point_down.y));
		dc->DrawLine(mouse->point_down, wxPoint(mouse->point_down.x, mouse->point_up.y));
		dc->DrawLine(mouse->point_up, wxPoint(mouse->point_down.x, mouse->point_up.y));
	}
}

p6::MainPanel::Item p6::MainPanel::get_item(real x, real y, uint *index) const
{
	const Construction *con = &_frame->construction;

	//Checking for clicking node
	for (uint i = 0; i < con->get_node_count(); i++)
	{
		if (pixels_in_meter * sqrt(
			sqr(con->get_node_x(i) - x) +
			sqr(con->get_force_y(i) - y)) < 5.0)
		{
			*index = i;
			return Item::node;
		}
	}

	//Checking for clicking stick
	for (uint i = 0; i < con->get_stick_count(); i++)
	{
		uint node[2];
		con->get_stick_node(i, node);
		real node_x[2], node_y[2];
		for (uint j = 0; j < 2; j++)
		{
			node_x[j] = con->get_node_x(node[j]);
			node_y[j] = con->get_node_y(node[j]);
		}
		
		if (Utils::distance_to_line(x, y, node_x, node_y) *pixels_in_meter < 5.0)
		{
			*index = i;
			return Item::stick;
		}
	}

	//Checking for clicking forces
	for (uint i = 0; i < con->get_force_count(); i++)
	{
		uint node = con->get_force_node(i);
		real node_x[2], node_y[2];
		node_x[0] = con->get_node_x(node);
		node_y[0] = con->get_node_y(node);
		node_x[1] = node_x[0] + con->get_force_x(i) * meters_in_newton;
		node_y[1] = node_y[0] + con->get_force_y(i) * meters_in_newton;

		if (Utils::distance_to_line(x, y, node_x, node_y) *pixels_in_meter < 5.0)
		{
			*index = i;
			return Item::force;
		}
	}

	return Item::no;
}

void p6::MainPanel::pixel_to_real(wxPoint point, real *x, real *y) const
{
	assert(x != nullptr);
	assert(y != nullptr);
	*x = center_x + (real)(point.x - panel->GetSize().x / 2) / pixels_in_meter;
	*y = center_y + (real)(panel->GetSize().y / 2 - point.y) / pixels_in_meter;
}

void p6::MainPanel::select_area(real ax, real ay, real bx, real by)
{
	selected_nodes.clear();
	selected_sticks.clear();
	selected_forces.clear();	
	if (bx < ax) { real b = ax; ax = bx; bx = b; }
	if (by < ay) { real b = ay; ay = by; by = b; }
	const Construction *con = &_frame->construction;

	for (uint i = 0; i < con->get_node_count(); i++)
	{
		if(ax < con->get_node_x(i) && con->get_node_x(i) < bx
		&& ay < con->get_node_y(i) && con->get_node_y(i) < by)
			selected_nodes.insert(i);
	}

	for (uint i = 0; i < con->get_stick_count(); i++)
	{
		uint node[2];
		con->get_stick_node(i, node);
		if (selected_nodes.count(node[0]) && selected_nodes.count(node[1]))
			selected_sticks.insert(i);
	}

	for (uint i = 0; i < con->get_force_count(); i++)
	{
		if (selected_nodes.count(con->get_force_node(i)))
			selected_forces.insert(i);
	}
}

void p6::MainPanel::refresh()
{
	panel->Refresh();
}