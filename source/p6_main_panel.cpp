/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_frame.hpp"

wxPoint p6::MainPanel::_real_to_pixel(Coord coord, wxPoint offset) const
{
	return offset + wxPoint(
		_panel->GetSize().x / 2 + (coord.x - center.x) * pixels_in_meter,
		_panel->GetSize().y / 2 - (coord.y - center.y) * pixels_in_meter);
}

p6::MainPanel::MainPanel(Frame *frame)
{
	_frame = frame;
	_panel = new wxPanel(frame->frame());
	_panel->Show(false);
	frame->sizer()->Add(_panel, 4, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN, 0);
}

wxPanel *p6::MainPanel::panel()
{
	return _panel;
}

wxSize p6::MainPanel::size() const
{
	return _panel->GetSize();
}

wxPoint p6::MainPanel::offset() const
{
	return _panel->GetClientAreaOrigin();
}

void p6::MainPanel::render(wxDC *dc, wxPoint offset) const
{
	bool sim = _frame->construction()->simulation();
	Construction *con = _frame->construction();

	//Calculate max strain
	real max_strain = 0.0;
	for (uint i = 0; i < con->get_stick_count(); i++)
	{
		if (con->get_stick_strain(i) > max_strain) max_strain = con->get_stick_strain(i);
	}

	//Draw sticks
	for (uint i = 0; i < _frame->construction()->get_stick_count(); i++)
	{
		uint node[2];
		_frame->construction()->get_stick_node(i, node);

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
			points[j] = _real_to_pixel(con->get_node_coord(node[j]), offset);
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
		const real angle = 30.0 * (M_PI / 180.0);
		const real coef = 0.1;

		Coord a = con->get_node_coord(con->get_force_node(i));
		Coord ba = con->get_force_direction(i) * meters_in_newton;
		Coord bc = Coord(
			cos(angle) * ba.x - sin(angle) * ba.y,
			sin(angle) * ba.x + cos(angle) * ba.y) * coef;
		Coord bd = Coord(
			cos(angle) * ba.x + sin(angle) * ba.y,
			-sin(angle) * ba.x + cos(angle) * ba.y) * coef;

		wxPoint pa = _real_to_pixel(a, offset);
		wxPoint pb = _real_to_pixel(a - ba, offset);
		wxPoint pc = _real_to_pixel(a - ba + bc, offset);
		wxPoint pd = _real_to_pixel(a - ba + bd, offset);
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
		wxPoint point = _real_to_pixel(con->get_node_coord(i), offset);
		if (selected_nodes.count(i)) dc->SetPen(wxPen(wxColour(255, 255, 0), 1));
		else dc->SetPen(wxPen(wxColour(0, 0, 0), 1));
		if (con->get_node_free(i)) dc->SetBrush(wxBrush(wxColour(128, 128, 128)));
		else dc->SetBrush(wxBrush(wxColour(64, 64, 64)));
		dc->DrawCircle(point, 5);
	}

	//Draw selection rect
	if (selected_area_draw)
	{
		dc->SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SHORT_DASH));
		dc->DrawLine(selected_area_points[1], wxPoint(selected_area_points[0].x, selected_area_points[1].y));
		dc->DrawLine(wxPoint(selected_area_points[0].x, selected_area_points[1].y), selected_area_points[0]);
		dc->DrawLine(selected_area_points[0], wxPoint(selected_area_points[1].x, selected_area_points[0].y));
		dc->DrawLine(wxPoint(selected_area_points[1].x, selected_area_points[0].y), selected_area_points[1]);
	}
}

p6::MainPanel::Item p6::MainPanel::get_item(wxPoint point) const
{
	const Construction *con = _frame->construction();
	Coord coord = pixel_to_real(point);

	//Checking for clicking node
	for (uint i = 0; i < con->get_node_count(); i++)
	{
		if (coord.distance(con->get_node_coord(i)) * pixels_in_meter < 5.0)
		{
			Item item;
			item.type = Item::Type::node;
			item.index = i;
			return item;
		}
	}

	//Checking for clicking stick
	for (uint i = 0; i < con->get_stick_count(); i++)
	{
		uint node[2];
		con->get_stick_node(i, node);
		Coord coords[2];
		coords[0] = con->get_node_coord(node[0]);
		coords[1] = con->get_node_coord(node[1]);
		if (coord.distance(coords) *pixels_in_meter < 5.0)
		{
			Item item;
			item.type = Item::Type::stick;
			item.index = i;
			return item;
		}
	}

	//Checking for clicking forces
	for (uint i = 0; i < con->get_force_count(); i++)
	{
		uint node = con->get_force_node(i);
		Coord coords[2];
		coords[0] = con->get_node_coord(node);
		coords[1] = coords[0] + con->get_force_direction(i) * meters_in_newton;
		if (coord.distance(coords) * pixels_in_meter < 5.0)
		{
			Item item;
			item.type = Item::Type::force;
			item.index = i;
			return item;
		}
	}

	Item item;
	item.type = Item::Type::no;
	return item;
}

p6::Coord p6::MainPanel::pixel_to_real(wxPoint point) const
{
	return Coord(
		center.x + (real)(point.x - _panel->GetSize().x / 2) / pixels_in_meter,
		center.y + (real)(_panel->GetSize().y / 2 - point.y) / pixels_in_meter);
}

void p6::MainPanel::select_items()
{
	selected_nodes.clear();
	selected_sticks.clear();
	selected_forces.clear();
	Coord coords[2];
	coords[0] = pixel_to_real(selected_area_points[0]);
	coords[1] = pixel_to_real(selected_area_points[1]);
	if (coords[0].x > coords[1].x) { wxCoord b = coords[1].x; coords[1].x = coords[0].x; coords[0].x = b; }
	if (coords[0].y > coords[1].y) { wxCoord b = coords[1].y; coords[1].y = coords[0].y; coords[0].y = b; }
	const Construction *con = _frame->construction();

	for (uint i = 0; i < con->get_node_count(); i++)
	{
		if(coords[0].x < con->get_node_coord(i).x && con->get_node_coord(i).x < coords[1].x
		&& coords[0].y < con->get_node_coord(i).y && con->get_node_coord(i).y < coords[1].y)
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

void p6::MainPanel::need_refresh()
{
	_panel->Refresh();
}