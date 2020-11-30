/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_construction.hpp"
#include "../header/p6_linear_material.hpp"
#include "../header/p6_nonlinear_material.hpp"
#include "../header/p6_file.hpp"
#include <cassert>
#include <Eigen>

class p6::Construction::Vector : public Eigen::Vector<p6::real, Eigen::Dynamic>
{
public:
	using Eigen::Vector<p6::real, Eigen::Dynamic>::Vector;
};

class p6::Construction::Matrix : public Eigen::Matrix<p6::real, Eigen::Dynamic, Eigen::Dynamic> {};

p6::uint p6::Construction::create_node() noexcept
{
	assert(!_simulation);
	Node node;
	node.freedom = 0;
	node.coord = Coord(0.0, 0.0);
	node.angle = 0.0;
	node.coord_simulated = Coord(0.0, 0.0);
	_node.push_back(node);
	return _node.size() - 1;
}

void p6::Construction::delete_node(uint node) noexcept
{
	assert(!_simulation);
	for (uint i = _stick.size() - 1; i != (uint)-1; i--)
	{
		if (_stick[i].node[0] == node || _stick[i].node[1] == node)
		{
			_stick.erase(_stick.begin() + i);
		}
		else
		{
			if (_stick[i].node[0] > node) _stick[i].node[0]--;
			if (_stick[i].node[1] > node) _stick[i].node[1]--;
		}
	}
	for (uint i = _force.size() - 1; i != (uint)-1; i--)
	{
		if (_force[i].node == node) _force.erase(_force.begin() + i);
		else if (_force[i].node > node) _force[i].node--;
	}
	_node.erase(_node.begin() + node);
}

void p6::Construction::set_node_coord(uint node, Coord coord) noexcept
{
	assert(!_simulation);
	assert(coord.x == coord.x);
	assert(abs(coord.x) != std::numeric_limits<real>::infinity());
	assert(coord.y == coord.y);
	assert(abs(coord.y) != std::numeric_limits<real>::infinity());
	_node[node].coord = coord;
}

void p6::Construction::set_node_freedom(uint node, unsigned char freedom) noexcept
{
	assert(!_simulation);
	assert(freedom <= 2);
	_node[node].freedom = freedom;
}

void p6::Construction::set_node_rail_angle(uint node, real angle) noexcept
{
	assert(!_simulation);
	assert(_node[node].freedom == 1);
	assert(angle == angle);
	assert(abs(angle) != std::numeric_limits<real>::infinity());
	_node[node].angle = angle;
}

p6::uint p6::Construction::get_node_count() const noexcept
{
	return _node.size();
}

p6::Coord p6::Construction::get_node_coord(uint node) const noexcept
{
	return _simulation ? _node[node].coord_simulated : _node[node].coord;
}

unsigned char p6::Construction::get_node_freedom(uint node) const noexcept
{
	return _node[node].freedom;
}

p6::real p6::Construction::get_node_rail_angle(uint node) const noexcept
{
	assert(_node[node].freedom == 1);
	return _node[node].angle;
}

p6::uint p6::Construction::create_stick(const uint node[2]) noexcept
{
	assert(!_simulation);
	assert(node[0] != node[1]);
	assert(node[0] < _node.size());
	assert(node[1] < _node.size());
	for (uint i = 0; i < _stick.size(); i++)
	{
		if((_stick[i].node[0] == node[0] && _stick[i].node[0] == node[1])
		&& (_stick[i].node[1] == node[1] && _stick[i].node[1] == node[1]))
			return i;
	}

	Stick stick;
	stick.node[0] = node[0];
	stick.node[1] = node[1];
	stick.material = (uint)-1;
	stick.area = 0.0;
	_stick.push_back(stick);
	return _stick.size() - 1;
}

void p6::Construction::delete_stick(uint stick) noexcept
{
	assert(!_simulation);
	_stick.erase(_stick.begin() + stick);
}

void p6::Construction::set_stick_material(uint stick, uint material) noexcept
{
	assert(!_simulation);
	_stick[stick].material = material;
}

void p6::Construction::set_stick_area(uint stick, real area) noexcept
{
	assert(!_simulation);
	assert(area == area);
	_stick[stick].area = area;
}

p6::uint p6::Construction::get_stick_count() const noexcept
{
	return _stick.size();
}

p6::uint p6::Construction::get_stick_material(uint stick) const noexcept
{
	return _stick[stick].material;
}

p6::real p6::Construction::get_stick_area(uint stick) const noexcept
{
	return _stick[stick].area;
}

void p6::Construction::get_stick_node(uint stick, uint node[2]) const noexcept
{
	node[0] = _stick[stick].node[0];
	node[1] = _stick[stick].node[1];
}

p6::real p6::Construction::get_stick_length(uint stick) const noexcept
{
	const Node *node[2] = { &_node[_stick[stick].node[0]], &_node[_stick[stick].node[1]] };
	return _simulation ?
		node[0]->coord_simulated.distance(node[1]->coord_simulated) :
		node[0]->coord.distance(node[1]->coord);
}

p6::real p6::Construction::get_stick_strain(uint stick) const noexcept
{
	assert(_simulation);
	const Node *node[2] = { &_node[_stick[stick].node[0]], &_node[_stick[stick].node[1]] };
	return (
		node[0]->coord_simulated.distance(node[1]->coord_simulated) /
		node[0]->coord.distance(node[1]->coord)
		) - 1.0;
}

p6::real p6::Construction::get_stick_force(uint stick) const noexcept
{
	assert(_simulation);
	return _stick[stick].area * _material[_stick[stick].material]->stress(get_stick_strain(stick));
}

p6::uint p6::Construction::create_force(uint node) noexcept
{
	assert(!_simulation);
	assert(node < _node.size());
	Force force;
	force.node = node;
	force.direction = Coord(0.0, 0.0);
	_force.push_back(force);
	return _force.size() - 1;
}

void p6::Construction::delete_force(uint force) noexcept
{
	assert(!_simulation);
	_force.erase(_force.begin() + force);
}

void p6::Construction::set_force_direction(uint force, Coord direction) noexcept
{
	assert(!_simulation);
	assert(direction.x == direction.x);
	assert(direction.y == direction.y);
	_force[force].direction = direction;
}

p6::uint p6::Construction::get_force_count() const noexcept
{
	return _force.size();
}

p6::Coord p6::Construction::get_force_direction(uint force) const noexcept
{
	return _force[force].direction;
}

p6::uint p6::Construction::get_force_node(uint force) const noexcept
{
	return _force[force].node;
}

p6::uint p6::Construction::create_linear_material(const String name, real modulus)
{
	assert(!_simulation);
	for (uint i = 0; i < _material.size(); i++)
	{
		if (name == _material[i]->name())
		{
			Material *material = new LinearMaterial(name, modulus);
			delete _material[i];
			_material[i] = material;
			return i;
		}
	}
	_material.push_back(new LinearMaterial(name, modulus));
	return _material.size() - 1;
}

p6::uint p6::Construction::create_nonlinear_material(const String name, const String formula)
{
	assert(!_simulation);
	for (uint i = 0; i < _material.size(); i++)
	{
		if (name == _material[i]->name())
		{
			Material *material = new NonlinearMaterial(name, formula);
			delete _material[i];
			_material[i] = material;
			return i;
		}
	}

	_material.push_back(new NonlinearMaterial(name, formula));
	return _material.size() - 1;
}

void p6::Construction::delete_material(uint material) noexcept
{
	assert(!_simulation);
	for (uint i = 0; i < _stick.size(); i++)
	{
		if (_stick[i].material == material) _stick[i].material = (uint)-1;
	}
	delete _material[material];
	_material.erase(_material.begin() + material);
}

p6::uint p6::Construction::get_material_count() const noexcept
{
	return _material.size();
}

p6::String p6::Construction::get_material_name(uint material) const noexcept
{
	return _material[material]->name();
}

p6::Material::Type p6::Construction::get_material_type(uint material) const noexcept
{
	return _material[material]->type();
}

p6::real p6::Construction::get_material_modulus(uint material) const noexcept
{
	assert(_material[material]->type() == Material::Type::linear);
	return ((LinearMaterial*)_material[material])->modulus();
}

p6::String p6::Construction::get_material_formula(uint material) const noexcept
{
	assert(_material[material]->type() == Material::Type::nonlinear);
	return ((NonlinearMaterial*)_material[material])->formula();
}

void p6::Construction::save(const String filepath) const
{
	//Open file
	OutputFile file(filepath);
	if (!file.ok()) throw std::runtime_error("File cannot be opened for write");
	Header header;
	header.node = _node.size();
	header.stick = _stick.size();
	header.force = _force.size();
	header.material = _material.size();
	file.write(&header, sizeof(Header));

	//Nodes
	for (uint i = 0; i < _node.size(); i++)
	{
		file.write(&_node[i], sizeof(StaticNode));
	}

	//Sticks
	for (uint i = 0; i < _stick.size(); i++)
	{
		file.write(&_stick[i], sizeof(Stick));
	}

	//Forces
	for (uint i = 0; i < _force.size(); i++)
	{
		file.write(&_force[i], sizeof(Force));
	}

	//Materials
	for (uint i = 0; i < _material.size(); i++)
	{
		//Name
		String name = _material[i]->name();
		uint len = name.size();
		file.write(&len, sizeof(uint));
		file.write(name.data(), len);

		//Type
		Material::Type type = _material[i]->type();
		file.write(&type, sizeof(Material::Type));

		if (type == Material::Type::linear)
		{
			//Modulus
			real modulus = ((LinearMaterial*)_material[i])->modulus();
			file.write(&modulus, sizeof(real));
		}
		else
		{
			//Formula
			String formula = ((NonlinearMaterial*)_material[i])->formula();
			len = formula.size();
			file.write(&len, sizeof(uint));
			file.write(formula.data(), len);
		}
	}
}

void p6::Construction::load(const String filepath)
{
	//Open file
	InputFile file(filepath);
	if (!file.ok()) throw std::runtime_error("File cannot be opened for read");
	Header header, sample;
	file.read(&header, sizeof(Header));
	if (memcmp(header.signature, sample.signature, 8) != 0) throw ("Invalid file format");
	
	//Nodes
	_node.resize(header.node);
	for (uint i = 0; i < _node.size(); i++)
	{
		file.read(&_node[i], sizeof(StaticNode));
	}

	//Sticks
	_stick.resize(header.stick);
	for (uint i = 0; i < _stick.size(); i++)
	{
		file.read(&_stick[i], sizeof(Stick));
	}

	//Forces
	_force.resize(header.force);
	for (uint i = 0; i < _force.size(); i++)
	{
		file.read(&_force[i], sizeof(Force));
	}

	//Materials
	for (uint i = 0; i < _material.size(); i++) delete _material[i];
	_material.resize(header.material);
	for (uint i = 0; i < _material.size(); i++)
	{
		//Name
		uint len;
		file.read(&len, sizeof(uint));
		String name(len, '\0');
		file.read(&name[0], len);

		//Type
		Material::Type type;
		file.read(&type, sizeof(Material::Type));

		if (type == Material::Type::linear)
		{
			//Modulus
			real modulus;
			file.read(&modulus, sizeof(real));
			_material[i] = new LinearMaterial(name, modulus);
		}
		else
		{
			//Formula
			file.read(&len, sizeof(uint));
			String formula(len, '\0');
			file.read(&formula[0], len);
			_material[i] = new NonlinearMaterial(name, formula);
		}
	}
}

void p6::Construction::import(const String filepath)
{
	//Open file
	InputFile file(filepath);
	if (!file.ok()) throw std::runtime_error("File cannot be opened for read");
	Header header, sample;
	file.read(&header, sizeof(Header));
	if (memcmp(header.signature, sample.signature, 8) != 0) throw ("Invalid file format");

	uint old_node_size = _node.size();
	uint old_stick_size = _stick.size();
	uint old_force_size = _force.size();
	uint old_material_size = _material.size();

	//Nodes
	_node.resize(old_node_size + header.node);
	for (uint i = old_node_size; i < _node.size(); i++)
	{
		file.read(&_node[i], sizeof(StaticNode));
	}

	//Sticks
	_stick.resize(old_stick_size + header.stick);
	for (uint i = old_stick_size; i < _stick.size(); i++)
	{
		file.read(&_stick[i], sizeof(Stick));
		_stick[i].node[0] += old_node_size;
		_stick[i].node[1] += old_node_size;
		_stick[i].material += old_material_size;
	}

	//Forces
	_force.resize(old_force_size + header.force);
	for (uint i = old_force_size; i < _force.size(); i++)
	{
		file.read(&_force[i], sizeof(Force));
		_force[i].node += old_node_size;
	}

	//Materials
	for (uint i = 0; i < header.material; i++)
	{
		//Name
		uint len;
		file.read(&len, sizeof(uint));
		String name(len, '\0');
		file.read(&name[0], len);

		//Find existing material
		uint existing_material = (uint)-1;
		for (uint j = 0; j < old_material_size; j++)
		{
			if (_material[j]->name() == name)
			{
				existing_material = j;
				break;
			}
		}

		//Type
		Material::Type type;
		file.read(&type, sizeof(Material::Type));

		//Creating material
		Material *new_material;
		if (type == Material::Type::linear)
		{
			//Modulus
			real modulus;
			file.read(&modulus, sizeof(real));
			if (existing_material == (uint)-1) new_material = new LinearMaterial(name, modulus);
		}
		else
		{
			//Formula
			file.read(&len, sizeof(uint));
			String formula(len, '\0');
			file.read(&formula[0], len);
			if (existing_material == (uint)-1) new_material = new NonlinearMaterial(name, formula);
		}

		//Adding material to materials
		if (existing_material == (uint)-1)
		{
			_material.push_back(new_material);
			existing_material = _material.size() - 1;
		}

		//Correcting sticks
		for (uint i = old_stick_size; i < _stick.size(); i++)
		{
			if (_stick[i].material == old_material_size + i) _stick[i].material = existing_material;
		}
	}
}

p6::uint p6::Construction::_node_equation_fx(uint free2d) const noexcept
{
	assert(free2d < _nfree2d);
	return 2 * free2d;
}

p6::uint p6::Construction::_node_equation_fy(uint free2d) const noexcept
{
	assert(free2d < _nfree2d);
	return 2 * free2d + 1;
}

p6::uint p6::Construction::_node_equation_fr(uint free1d) const noexcept
{
	assert(free1d < _nfree1d);
	return 2 * _nfree2d + free1d;
}

p6::uint p6::Construction::_equation_number() const noexcept
{
	return 2 * _nfree2d + _nfree1d;
}

p6::uint p6::Construction::_node_variable_x(uint free2d) const noexcept
{
	assert(free2d < _nfree2d);
	return 2 * free2d;
}

p6::uint p6::Construction::_node_variable_y(uint free2d) const noexcept
{
	assert(free2d < _nfree2d);
	return 2 * free2d + 1;
}

p6::uint p6::Construction::_node_variable_r(uint free1d) const noexcept
{
	assert(free1d < _nfree1d);
	return 2 * _nfree2d + free1d;
}

p6::uint p6::Construction::_variable_number() const noexcept
{
	return 2 * _nfree2d + _nfree1d;
}

void p6::Construction::_check_materials_specified() const
{
	for (uint i = 0; i < _stick.size(); i++)
	{
		assert(_stick[i].material < _material.size() || _stick[i].material == (uint)-1);
		if (_stick[i].material == (uint)-1) throw std::runtime_error("Material is not specified");
	}
}

void p6::Construction::_create_map(std::vector<uint> *node_to_free) noexcept
{
	_nfree2d = 0;
	_nfree1d = 0;
	node_to_free->resize(_node.size(), (uint)-1);
	for (uint i = 0; i < _node.size(); i++)
	{
		if (_node[i].freedom == 1)
		{
			node_to_free->at(i) = _nfree1d;
			_nfree1d++;
		}
		else if (_node[i].freedom == 2)
		{
			node_to_free->at(i) = _nfree2d;
			_nfree2d++;
		}
	}
}

p6::real p6::Construction::_get_tolerance() const noexcept
{
	real minforce = std::numeric_limits<real>::infinity();
	for (uint i = 0; i < _force.size(); i++)
	{
		real newforce = _force[i].direction.norm();
		if (newforce < minforce) minforce = newforce;
	}
	return minforce / 1000.0;
}

void p6::Construction::_create_vectors(
	const std::vector <uint> *node_to_free,
	Vector *s,
	Vector *z,
	Vector *m,
	Matrix *d) const noexcept
{
	s->resize(_variable_number());
	for (uint i = 0; i < _node.size(); i++)
	{
		if (_node[i].freedom == 1)
		{
			uint free1d = node_to_free->at(i);
			(*s)(_node_variable_r(free1d)) = 0.0;
		}
		else if (_node[i].freedom == 2)
		{
			uint free2d = node_to_free->at(i);
			(*s)(_node_variable_x(free2d)) = _node[i].coord.x;
			(*s)(_node_variable_y(free2d)) = _node[i].coord.y;
		}
	}
	z->resize(_equation_number());
	m->resize(_equation_number());
	d->resize(_equation_number(), _variable_number());
}

void p6::Construction::_set_z_to_external_forces(
	const std::vector <uint> *node_to_free,
	Vector *z) const noexcept
{
	for (uint i = 0; i < _force.size(); i++)
	{
		uint node = _force[i].node;
		if (_node[node].freedom == 1)
		{
			uint free1d = node_to_free->at(node);
			real angle = _node[node].angle;
			(*z)(_node_variable_r(free1d)) = _force[i].direction.x * cos(angle) + _force[i].direction.y * sin(angle);
		}
		else if (_node[node].freedom == 2)
		{
			uint free2d = node_to_free->at(node);
			(*z)(_node_variable_x(free2d)) = _force[i].direction.x;
			(*z)(_node_variable_y(free2d)) = _force[i].direction.y;
		}
	}
}

void p6::Construction::_set_d_to_zero(
	const std::vector <uint> *node_to_free,
	Matrix *d) const noexcept
{
	for (uint i = 0; i < _stick.size(); i++)
	{
		const uint *node = _stick[i].node;
		for (uint j = 0; j < 2; j++)
		{
			//If current point is fixed on rail
			if (_node[node[j]].freedom == 1)
			{
				//It sets own derivatives on own coordinates
				uint free1d = node_to_free->at(node[j]);
				(*d)(_node_equation_fr(free1d), _node_variable_r(free1d)) = 0.0;

				//And own derivatives on coordinates of other point
				if (_node[node[j ^ 1]].freedom == 1)
				{
					uint other_free1d = node_to_free->at(node[j]);
					(*d)(_node_equation_fr(free1d), _node_variable_r(other_free1d)) = 0.0;
				}
				else if (_node[node[j ^ 1]].freedom == 2)
				{
					uint other_free2d = node_to_free->at(node[j]);
					(*d)(_node_equation_fr(free1d), _node_variable_x(other_free2d)) = 0.0;
					(*d)(_node_equation_fr(free1d), _node_variable_y(other_free2d)) = 0.0;
				}
			}
			//If current point is free
			else if (_node[node[j]].freedom == 2)
			{
				//It sets own derivatives on own coordinates
				uint free2d = node_to_free->at(node[j]);
				(*d)(_node_equation_fx(free2d), _node_variable_x(free2d)) = 0.0;
				(*d)(_node_equation_fx(free2d), _node_variable_y(free2d)) = 0.0;
				(*d)(_node_equation_fy(free2d), _node_variable_x(free2d)) = 0.0;
				(*d)(_node_equation_fy(free2d), _node_variable_y(free2d)) = 0.0;

				//And own derivatives on coordinates of other point
				if (_node[node[j ^ 1]].freedom == 1)
				{
					uint other_free1d = node_to_free->at(node[j]);
					(*d)(_node_equation_fx(free2d), _node_variable_r(other_free1d)) = 0.0;
					(*d)(_node_equation_fy(free2d), _node_variable_r(other_free1d)) = 0.0;
				}
				else if (_node[node[j ^ 1]].freedom == 2)
				{
					uint other_free2d = node_to_free->at(node[j]);
					(*d)(_node_equation_fx(free2d), _node_variable_x(other_free2d)) = 0.0;
					(*d)(_node_equation_fx(free2d), _node_variable_y(other_free2d)) = 0.0;
					(*d)(_node_equation_fy(free2d), _node_variable_x(other_free2d)) = 0.0;
					(*d)(_node_equation_fy(free2d), _node_variable_y(other_free2d)) = 0.0;
				}
			}
		}
	}
}

p6::Coord p6::Construction::_get_delta(
	uint stick,
	const std::vector <uint> *node_to_free,
	const Vector *s) const noexcept
{
	const uint *node = _stick[stick].node;
	Coord coord[2];
	for (uint i = 0; i < 2; i++)
	{
		if (_node[node[i]].freedom == 1)
		{
			uint free1d = node_to_free->at(node[i]);
			real angle = _node[node[i]].angle;
			coord[i] = _node[node[i]].coord + Coord(cos(angle), sin(angle)) * (*s)(_node_variable_r(free1d));
		}
		else if (_node[node[i]].freedom == 2)
		{
			uint free2d = node_to_free->at(node[i]);
			coord[i] = Coord((*s)(_node_variable_x(free2d)), (*s)(_node_variable_y(free2d)));
		}
		else coord[i] = _node[node[i]].coord;
	}
	return coord[1] - coord[0];
}

void p6::Construction::_modify_z_with_stick_force(
	uint stick,
	const std::vector <uint> *node_to_free,
	const Vector *s,
	Vector *z) const noexcept
{
	const Material *material = _material[_stick[stick].material];
	const uint *node = _stick[stick].node;
	Coord delta = _get_delta(stick, node_to_free, s);
	real length = delta.norm();
	real initial_length = _node[node[0]].coord.distance(_node[node[1]].coord);
	real force = _stick[stick].area * material->stress(length / initial_length - 1.0);

	for (uint i = 0; i < 2; i++)
	{
		if (_node[node[i]].freedom == 1)
		{
			uint free1d = node_to_free->at(node[i]);
			real sign = i == 0 ? 1.0 : -1.0;
			(*z)(_node_variable_r(free1d)) +=
				cos(_node[node[i]].angle) * sign * force * delta.x / length +
				sin(_node[node[i]].angle) * sign * force * delta.y / length;
		}
		else if (_node[node[i]].freedom == 2)
		{
			uint free2d = node_to_free->at(node[i]);
			real sign = i == 0 ? 1.0 : -1.0;
			(*z)(_node_variable_x(free2d)) += sign * force * delta.x / length;
			(*z)(_node_variable_y(free2d)) += sign * force * delta.y / length;
		}
	}
}

void p6::Construction::_modify_d_with_stick_force(
	uint stick,
	const std::vector <uint> *node_to_free,
	const Vector *s,
	Matrix *d) const noexcept
{
	const uint *node = _stick[stick].node;
	const Material *material = _material[_stick[stick].material];
	Coord delta = _get_delta(stick, node_to_free, s);
	real length = delta.norm();
	real initial_length = _node[node[0]].coord.distance(_node[node[1]].coord);
	real strain = length / initial_length - 1.0;
	real force = _stick[stick].area * material->stress(strain);

	for (uint i = 0; i < 2; i++)
	{
		Coord deltaoi = (i == 0) ? delta : delta * (-1.0);

		//If current point is fixed on rail
		if (_node[node[i]].freedom == 1)
		{
			//It sets own derivatives on own coordinates
			uint free1d = node_to_free->at(node[i]);
			real anglei = _node[node[i]].angle;
			real dl_dri = (-deltaoi.x * cos(anglei) - deltaoi.y * sin(anglei)) / length;
			real df_dri = _stick[stick].area * material->derivative(strain) * dl_dri / initial_length;
			(*d)(_node_equation_fr(free1d), _node_variable_r(free1d)) += (
				cos(anglei) * ((df_dri * deltaoi.x + force * (-cos(anglei))) * length - dl_dri * force * deltaoi.x) +
				sin(anglei) * ((df_dri * deltaoi.y + force * (-sin(anglei))) * length - dl_dri * force * deltaoi.y)
				) / sqr(length);

			//And own derivatives on coordinates of other point
			if (_node[node[i ^ 1]].freedom == 1)
			{
				uint other_free1d = node_to_free->at(node[i]);
				real angleo = _node[node[i ^ 1]].angle;
				real dl_dro = (deltaoi.x * cos(angleo) + deltaoi.y * sin(angleo)) / length;
				real df_dro = _stick[stick].area * material->derivative(strain) * dl_dro / initial_length;
				(*d)(_node_equation_fr(free1d), _node_variable_r(other_free1d)) += (
					cos(anglei) * ((df_dro * deltaoi.x + force * cos(angleo)) * length - dl_dro * force * deltaoi.x) +
					sin(anglei) * ((df_dro * deltaoi.y + force * sin(angleo)) * length - dl_dro * force * deltaoi.y)
					) / sqr(length);
			}
			else if (_node[node[i ^ 1]].freedom == 2)
			{
				uint other_free2d = node_to_free->at(node[i]);
				real dl_dxo = deltaoi.x / length;
				real df_dxo = _stick[stick].area * material->derivative(strain) * dl_dxo / initial_length;
				(*d)(_node_equation_fr(free1d), _node_variable_x(other_free2d)) += (
					cos(anglei) * ((df_dxo * deltaoi.x + force * 1.0) * length - dl_dxo * force * deltaoi.x) +
					sin(anglei) * deltaoi.y * (df_dxo * length - dl_dxo * force)
					) / sqr(length);
				real dl_dyo = deltaoi.y / length;
				real df_dyo = _stick[stick].area * material->derivative(strain) * dl_dyo / initial_length;
				(*d)(_node_equation_fr(free1d), _node_variable_y(other_free2d)) += (
					cos(anglei) * deltaoi.x * (df_dyo * length - dl_dyo * force) +
					sin(anglei) * ((df_dyo * deltaoi.y + force * 1.0) * length - dl_dyo * force * deltaoi.y)
					) / sqr(length);
			}
		}
		//If current point is free
		else if (_node[node[i]].freedom == 2)
		{
			//It sets own derivatives on own coordinates
			uint free2d = node_to_free->at(node[i]);
			
			real dl_dxi = -deltaoi.x / length;
			real df_dxi = _stick[stick].area * material->derivative(strain) * dl_dxi / initial_length;
			real dfxi_dxi = ((df_dxi * deltaoi.x + force * (-1.0)) * length - dl_dxi * force * deltaoi.x) / sqr(length);
			(*d)(_node_equation_fx(free2d), _node_variable_x(free2d)) += dfxi_dxi;
			real dl_dyi = -deltaoi.y / length;
			real df_dyi = _stick[stick].area * material->derivative(strain) * dl_dyi / initial_length;
			real dfxi_dyi = deltaoi.x * (df_dyi * length - dl_dyi * force) / sqr(length);
			(*d)(_node_equation_fx(free2d), _node_variable_y(free2d)) += dfxi_dyi;
			real dfyi_dxi = deltaoi.y * (df_dxi * length - dl_dxi * force) / sqr(length);
			(*d)(_node_equation_fy(free2d), _node_variable_x(free2d)) += dfyi_dxi;
			real dfyi_dyi = ((df_dyi * deltaoi.y + force * (-1.0)) * length - dl_dyi * force * deltaoi.y) / sqr(length);
			(*d)(_node_equation_fy(free2d), _node_variable_y(free2d)) += dfyi_dyi;

			//And own derivatives on coordinates of other point
			if (_node[node[i ^ 1]].freedom == 1)
			{
				uint other_free1d = node_to_free->at(node[i]);
				real angleo = _node[node[i ^ 1]].angle;
				real dl_dro = (deltaoi.x * cos(angleo) + deltaoi.y * sin(angleo)) / length;
				real df_dro = _stick[stick].area * material->derivative(strain) * dl_dro / initial_length;
				(*d)(_node_equation_fx(free2d), _node_variable_r(other_free1d)) +=
					((df_dro * deltaoi.x + force * cos(angleo)) * length - dl_dro * force * deltaoi.x) / sqr(length);
				(*d)(_node_equation_fy(free2d), _node_variable_r(other_free1d)) +=
					((df_dro * deltaoi.y + force * sin(angleo)) * length - dl_dro * force * deltaoi.y) / sqr(length);
			}
			else if (_node[node[i ^ 1]].freedom == 2)
			{
				uint other_free2d = node_to_free->at(node[i]);
				(*d)(_node_equation_fx(free2d), _node_variable_x(other_free2d)) -= dfxi_dxi;
				(*d)(_node_equation_fx(free2d), _node_variable_y(other_free2d)) -= dfxi_dyi;
				(*d)(_node_equation_fy(free2d), _node_variable_x(other_free2d)) -= dfyi_dxi;
				(*d)(_node_equation_fy(free2d), _node_variable_y(other_free2d)) -= dfyi_dyi;
			}
		}
	}
}

p6::real p6::Construction::_get_residuum(const Vector *z) const noexcept
{
	real error = 0.0;
	for (int i = 0; i < z->rows(); i++)
	{
		real newabs = abs((*z)(i));
		if (newabs > error) error = newabs;
	}
	return error;
}

void p6::Construction::_apply_state_vector(
	const std::vector<uint> *node_to_free,
	const Vector *s) noexcept
{
	for (uint i = 0; i < _node.size(); i++)
	{
		if (_node[i].freedom == 1)
		{
			uint free1d = node_to_free->at(i);
			real angle = _node[i].angle;
			_node[i].coord_simulated = _node[i].coord + Coord(cos(angle), sin(angle)) * (*s)(_node_variable_r(free1d));
		}
		else if (_node[i].freedom == 2)
		{
			uint free2d = node_to_free->at(i);
			_node[i].coord_simulated = Coord((*s)(_node_variable_x(free2d)), (*s)(_node_variable_y(free2d)));
		}
		else _node[i].coord_simulated = _node[i].coord;
	}
}

bool p6::Construction::_is_adequat(const Vector *m) const noexcept
{
	bool adequat = true;
	for (int i = 0; i < m->rows(); i++)
	{
		if ((*m)(i) != (*m)(i) || abs((*m)(i)) == std::numeric_limits<real>::infinity())
			return false;
	}
	return true;
}

p6::real  p6::Construction::_get_flow_coefficient(
	const std::vector <uint> *node_to_free,
	const Vector *s,
	const Vector *z) const noexcept
{
	real coef = std::numeric_limits<real>::infinity();
	for (uint i = 0; i < _stick.size(); i++)
	{
		const uint *node = _stick[i].node;
		Coord delta = _get_delta(i, node_to_free, s);
		real length = delta.norm();
		real initial_length = _node[node[0]].coord.distance(_node[node[1]].coord);
		real strain = length / initial_length - 1.0;
		real df_dl = _stick[i].area * _material[_stick[i].material]->derivative(strain) / initial_length;
		if (1.0 / df_dl < coef) coef = 1.0 / df_dl;

		for (uint j = 0; j < 2; j++)
		{
			if (_node[node[j]].freedom == 1)
			{
				uint free1d = node_to_free->at(node[j]);
				real unbalanced_force = abs((*z)(_node_equation_fr(free1d)));
				if (length / unbalanced_force < coef) coef = length / unbalanced_force;		
			}
			else if (_node[node[j]].freedom == 2)
			{
				uint free2d = node_to_free->at(node[j]);
				real unbalanced_force = Coord((*z)(_node_equation_fx(free2d)), (*z)(_node_equation_fy(free2d))).norm();
				if (length / unbalanced_force < coef) coef = length / unbalanced_force;
			}
		}
	}
	return coef;
}

void p6::Construction::simulate(bool sim)
{
	if (sim == _simulation) return;
	else if (!sim) { _simulation = false; return; }

	//Checking if materials are specified
	_check_materials_specified();

	//Creating node-to-free map
	std::vector<uint> node_to_free;
	_create_map(&node_to_free);

	//Calculating tolerance
	real tolerance = _get_tolerance();

	//Creating vectors and matrixes
	Vector s;	//State vector
	Vector z;	//Should-be-zero value
	Vector m;	//Modification of state vector
	Matrix d;	//Derivative of should-be-zero value
	_create_vectors(&node_to_free, &s, &z, &m, &d);

	//Iterating
	real last_error = 0.0;
	uint not_converge_count = 0;
	while (true)
	{
		_set_z_to_external_forces(&node_to_free, &z);
		_set_d_to_zero(&node_to_free, &d);
		for (uint i = 0; i < _stick.size(); i++)
		{
			_modify_z_with_stick_force(i, &node_to_free, &s, &z);
			_modify_d_with_stick_force(i, &node_to_free, &s, &d);
		}
		real error = _get_residuum(&z);
		if (error < tolerance) break;
		else if (error < last_error) not_converge_count = 0;
		else if (++not_converge_count == 1000) throw std::runtime_error("Simulation does not converge");
		Eigen::HouseholderQR<Eigen::Matrix<real, Eigen::Dynamic, Eigen::Dynamic>> qr(d);
		m = qr.solve(z);
		if (_is_adequat(&m)) s -= m;
		else s += 0.001 * _get_flow_coefficient(&node_to_free, &s, &z) * z;
	}
	_apply_state_vector(&node_to_free, &s);

	_simulation = true;
}

p6::Construction::~Construction()
{
	for (uint i = 0; i < _material.size(); i++) delete _material[i];
}
