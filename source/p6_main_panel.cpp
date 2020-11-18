/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_main_panel.hpp"
#include "../header/p6_frame.hpp"

wxPoint p6::MainPanel::_real_to_pixel(Coord coord, wxPoint offset) const noexcept
{
	return offset + wxPoint(
		_panel->GetSize().x / 2 + (coord.x - _center.x) * pixels_in_meter,
		_panel->GetSize().y / 2 - (coord.y - _center.y) * pixels_in_meter);
}

p6::MainPanel::MainPanel(Frame *frame) noexcept :
	_frame(frame),
	_panel(new wxPanel(frame->frame()))
{
	_panel->Show(false);
	frame->sizer()->Add(_panel, 4, wxEXPAND | wxRESERVE_SPACE_EVEN_IF_HIDDEN, 0);
}

wxPanel *p6::MainPanel::panel() noexcept
{
	return _panel;
}

wxSize p6::MainPanel::size() const noexcept
{
	return _panel->GetSize();
}

void p6::MainPanel::render(wxDC *dc, wxPoint offset) const noexcept
{
	bool sim = _frame->toolbar()->simulation();
	Construction *con = _frame->construction();

	//Calculate max strain
	real max_strain = 0.0;
	if (sim)
	{
		for (uint i = 0; i < con->get_stick_count(); i++)
		{
			if (con->get_stick_strain(i) > max_strain) max_strain = con->get_stick_strain(i);
		}
	}

	//Draw sticks
	for (uint i = 0; i < _frame->construction()->get_stick_count(); i++)
	{
		uint node[2];
		_frame->construction()->get_stick_node(i, node);

		wxColour colour;
		if (sim)
		{
			real strain = con->get_stick_strain(i);
			if (strain == 0) colour = wxColour(128, 128, 128);
			else if (strain > 0)
			{
				//Blue
				real rg = std::max(128.0 - 128.0 * strain / max_strain, 0.0);
				real b = std::min(128.0 + 127 * strain / max_strain, 255.0);
				colour = wxColour(rg, rg, b);
			}
			else
			{
				//Red
				real gb = std::max(128.0 + 128 * strain / max_strain, 0.0);
				real r = std::min(128.0 - 127.0 * strain / max_strain, 255.0);
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
		Coord a = con->get_node_coord(con->get_force_node(i));
		Coord ba = con->get_force_direction(i) * meters_in_newton * (-1.0);
		Coord bc = ba.rotate(30.0 * (pi() / 180.0)) * 0.1;
		Coord bd = ba.rotate(-30.0 * (pi() / 180.0)) * 0.1;

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
	if (_area_select_draw)
	{
		dc->SetPen(wxPen(wxColour(0, 0, 0), 1, wxPENSTYLE_SHORT_DASH));
		dc->DrawLine(offset + _area_select_current, offset + wxPoint(_area_select_begin.x, _area_select_current.y));
		dc->DrawLine(offset + wxPoint(_area_select_begin.x, _area_select_current.y), offset + _area_select_begin);
		dc->DrawLine(offset + _area_select_begin, offset + wxPoint(_area_select_current.x, _area_select_begin.y));
		dc->DrawLine(offset + wxPoint(_area_select_current.x, _area_select_begin.y), offset + _area_select_current);
	}

	//Draw anchor
	if (_frame->toolbar()->tool() == ToolBar::Tool::move)
	{
		wxPoint point = _real_to_pixel(_frame->side_panel()->move_bar()->anchor(), offset);
		dc->SetPen(wxPen(wxColour(0, 0, 0), 1));
		dc->SetBrush(wxBrush(wxColour(255, 0, 0)));
		dc->DrawCircle(point, 5);
	}
}

p6::MainPanel::Item p6::MainPanel::get_item(wxPoint point) const noexcept
{
	const Construction *con = _frame->construction();
	Coord coord = pixel_to_real(point);

	//Checking for clicking anchor
	if (_frame->toolbar()->tool() == ToolBar::Tool::move)
	{
		if (coord.distance(_frame->side_panel()->move_bar()->anchor()) * pixels_in_meter < 5.0)
		{
			Item item;
			item.type = Item::Type::anchor;
			return item;
		}
	}

	//Checking for clicking node
	for (uint i = 0; i < con->get_node_count(); i++)
	{
		if (coord.distance(con->get_node_coord(i)) * pixels_in_meter < 5.0)
		{
			Item item;
			item.type = Item::Type::node;
			item.index = i;
			item.selected = selected_nodes.count(i);
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
			item.selected = selected_sticks.count(i);
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
			item.selected = selected_forces.count(i);
			return item;
		}
	}

	Item item;
	item.type = Item::Type::no;
	return item;
}

p6::Coord p6::MainPanel::pixel_to_real(wxPoint point) const noexcept
{
	return Coord(
		_center.x + (real)(point.x - _panel->GetSize().x / 2) / pixels_in_meter,
		_center.y + (real)(_panel->GetSize().y / 2 - point.y) / pixels_in_meter);
}

void p6::MainPanel::area_select_begin(wxPoint point) noexcept
{
	_area_select_begin = point;
	_area_select_current = point;
	_area_select_draw = true;
}

void p6::MainPanel::area_select_continue(wxPoint point) noexcept
{
	if (_area_select_current != point)
	{
		_area_select_current = point;
		need_refresh();
	}
}

void p6::MainPanel::area_select_end(wxPoint point) noexcept
{
	_area_select_draw = false;
	selected_nodes.clear();
	selected_sticks.clear();
	selected_forces.clear();
	Coord coords[2];
	coords[0] = pixel_to_real(_area_select_begin);
	coords[1] = pixel_to_real(point);
	if (coords[0].x > coords[1].x) { real b = coords[1].x; coords[1].x = coords[0].x; coords[0].x = b; }
	if (coords[0].y > coords[1].y) { real b = coords[1].y; coords[1].y = coords[0].y; coords[0].y = b; }
	const Construction *con = _frame->construction();

	for (uint i = 0; i < con->get_node_count(); i++)
	{
		if (coords[0].x < con->get_node_coord(i).x && con->get_node_coord(i).x < coords[1].x
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

	need_refresh();
	_frame->side_panel()->refresh();
}

void p6::MainPanel::drag_begin(wxPoint point) noexcept
{
	_old_center = _center;
	_drag_begin = point;
}

void p6::MainPanel::drag_continue(wxPoint point) noexcept
{
	_center.x = _old_center.x + (_drag_begin.x - point.x) / pixels_in_meter;
	_center.y = _old_center.y + (point.y - _drag_begin.y) / pixels_in_meter;
	need_refresh();
}

void p6::MainPanel::need_refresh() noexcept
{
	_frame->frame()->Refresh();
}