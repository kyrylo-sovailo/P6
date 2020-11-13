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
#include <wx/file.h>
#include <cassert>
#include <Eigen>

class p6::Construction::Vector : public Eigen::Vector<p6::real, Eigen::Dynamic> {};
class p6::Construction::Matrix : public Eigen::Matrix<p6::real, Eigen::Dynamic, Eigen::Dynamic> {};

p6::uint p6::Construction::create_node(Coord coord, bool free)
{
	assert(!_simulation);
	assert(coord.x == coord.x);
	assert(coord.y == coord.y);
	Node node;
	node.free = free;
	node.coord = coord;
	_node.push_back(node);
	return _node.size() - 1;
}

void p6::Construction::delete_node(uint node)
{
	assert(!_simulation);
	for (uint i = _stick.size() - 1; i != (uint)-1; i--)
	{
		if (_stick[i].node[0] == node || _stick[i].node[1] == node)
		{
			_stick.erase(_stick.cbegin() + i);
		}
		else
		{
			if (_stick[i].node[0] > node) _stick[i].node[0]--;
			if (_stick[i].node[1] > node) _stick[i].node[1]--;
		}
	}
	for (uint i = _force.size() - 1; i != (uint)-1; i--)
	{
		if (_force[i].node == node) _force.erase(_force.cbegin() + i);
		else if (_force[i].node > node) _force[i].node--;
	}
	_node.erase(_node.cbegin() + node);
}

void p6::Construction::set_node_coord(uint node, Coord coord)
{
	assert(!_simulation);
	assert(coord.x == coord.x);
	assert(coord.y == coord.y);
	_node[node].coord = coord;
}

void p6::Construction::set_node_free(uint node, bool free)
{
	assert(!_simulation);
	_node[node].free = free;
}

p6::uint p6::Construction::get_node_count() const
{
	return _node.size();
}

p6::Coord p6::Construction::get_node_coord(uint node) const
{
	return _simulation ? _node[node].coord_simulated : _node[node].coord;
}

bool p6::Construction::get_node_free(uint node) const
{
	return _node[node].free;
}

p6::uint p6::Construction::create_stick(const uint node[2], uint material, real area)
{
	assert(!_simulation);
	assert(area >= 0);
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
	stick.material = material;
	stick.area = area;
	_stick.push_back(stick);
	return _stick.size() - 1;
}

void p6::Construction::delete_stick(uint stick)
{
	assert(!_simulation);
	_stick.erase(_stick.cbegin() + stick);
}

void p6::Construction::set_stick_material(uint stick, uint material)
{
	assert(!_simulation);
	_stick[stick].material = material;
}

void p6::Construction::set_stick_area(uint stick, real area)
{
	assert(!_simulation);
	assert(area == area);
	_stick[stick].area = area;
}

p6::uint p6::Construction::get_stick_count() const
{
	return _stick.size();
}

p6::uint p6::Construction::get_stick_material(uint stick) const
{
	return _stick[stick].material;
}

p6::real p6::Construction::get_stick_area(uint stick) const
{
	return _stick[stick].area;
}

void p6::Construction::get_stick_node(uint stick, uint node[2]) const
{
	node[0] = _stick[stick].node[0];
	node[1] = _stick[stick].node[1];
}

p6::real p6::Construction::get_stick_length(uint stick) const
{
	const Node *node[2] = { &_node[_stick[stick].node[0]], &_node[_stick[stick].node[1]] };
	return _simulation ?
		node[0]->coord_simulated.distance(node[1]->coord_simulated) :
		node[0]->coord.distance(node[1]->coord);
}

p6::real p6::Construction::get_stick_strain(uint stick) const
{
	assert(_simulation);
	const Node *node[2] = { &_node[_stick[stick].node[0]], &_node[_stick[stick].node[1]] };
	return (
		node[0]->coord_simulated.distance(node[1]->coord_simulated) /
		node[0]->coord.distance(node[1]->coord)
		) - 1.0;
}

p6::real p6::Construction::get_stick_force(uint stick) const
{
	assert(_simulation);
	return _stick[stick].area * _material[_stick[stick].material]->stress(get_stick_strain(stick));
}

p6::uint p6::Construction::create_force(uint node, Coord direction)
{
	assert(!_simulation);
	assert(direction.x == direction.x);
	assert(direction.y == direction.y);
	assert(node < _node.size());
	Force force;
	force.node = node;
	force.direction = direction;
	_force.push_back(force);
	return _force.size() - 1;
}

void p6::Construction::delete_force(uint force)
{
	assert(!_simulation);
	_force.erase(_force.cbegin() + force);
}

void p6::Construction::set_force_direction(uint force, Coord direction)
{
	assert(!_simulation);
	assert(direction.x == direction.x);
	assert(direction.y == direction.y);
	_force[force].direction = direction;
}

p6::uint p6::Construction::get_force_count() const
{
	return _force.size();
}

p6::Coord p6::Construction::get_force_direction(uint force) const
{
	return _force[force].direction;
}

p6::uint p6::Construction::get_force_node(uint force) const
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

void p6::Construction::delete_material(uint material)
{
	assert(!_simulation);
	for (uint i = 0; i < _stick.size(); i++)
	{
		if (_stick[i].material == material) _stick[i].material = (uint)-1;
	}
	delete _material[material];
	_material.erase(_material.cbegin() + material);
}

p6::uint p6::Construction::get_material_count() const
{
	return _material.size();
}

p6::String p6::Construction::get_material_name(uint material) const
{
	return _material[material]->name();
}

p6::Material::Type p6::Construction::get_material_type(uint material) const
{
	return _material[material]->type();
}

p6::real p6::Construction::get_material_modulus(uint material) const
{
	assert(_material[material]->type() == Material::Type::linear);
	return ((LinearMaterial*)_material[material])->modulus();
}

p6::String p6::Construction::get_material_formula(uint material) const
{
	assert(_material[material]->type() == Material::Type::nonlinear);
	return ((NonlinearMaterial*)_material[material])->formula();
}

void p6::Construction::save(const String filepath) const
{
	//Open file
	wxFile file(filepath, wxFile::OpenMode::write);
	if (!file.IsOpened()) throw std::runtime_error("File cannot be opened for write");
	Header header;
	header.node = _node.size();
	header.stick = _stick.size();
	header.force = _force.size();
	header.material = _material.size();
	file.Write(&header, sizeof(Header));

	//Nodes
	for (uint i = 0; i < _node.size(); i++)
	{
		file.Write(&_node[i], sizeof(StaticNode));
	}

	//Sticks
	for (uint i = 0; i < _stick.size(); i++)
	{
		file.Write(&_stick[i], sizeof(Stick));
	}

	//Forces
	for (uint i = 0; i < _force.size(); i++)
	{
		file.Write(&_force[i], sizeof(Force));
	}

	//Materials
	for (uint i = 0; i < _material.size(); i++)
	{
		//Name
		String name = _material[i]->name();
		uint len = name.size();
		file.Write(&len, sizeof(uint));
		file.Write(name.data(), len);

		//Type
		Material::Type type = _material[i]->type();
		file.Write(&type, sizeof(Material::Type));

		if (type == Material::Type::linear)
		{
			//Modulus
			real modulus = ((LinearMaterial*)_material[i])->modulus();
			file.Write(&modulus, sizeof(real));
		}
		else
		{
			//Formula
			String formula = ((NonlinearMaterial*)_material[i])->formula();
			len = formula.size();
			file.Write(&len, sizeof(uint));
			file.Write(formula.data(), len);
		}
	}
}

void p6::Construction::load(const String filepath)
{
	//Open file
	wxFile file(filepath, wxFile::OpenMode::read);
	if (!file.IsOpened()) throw std::runtime_error("File cannot be opened for read");
	Header header, sample;
	file.Read(&header, sizeof(Header));
	if (memcmp(header.signature, sample.signature, 8) != 0) throw ("Invalid file format");
	
	//Nodes
	_node.resize(header.node);
	for (uint i = 0; i < _node.size(); i++)
	{
		file.Read(&_node[i], sizeof(StaticNode));
	}

	//Sticks
	_stick.resize(header.stick);
	for (uint i = 0; i < _stick.size(); i++)
	{
		file.Read(&_stick[i], sizeof(Stick));
	}

	//Forces
	_force.resize(header.force);
	for (uint i = 0; i < _force.size(); i++)
	{
		file.Read(&_force[i], sizeof(Force));
	}

	//Materials
	for (uint i = 0; i < _material.size(); i++) delete _material[i];
	_material.resize(header.material);
	for (uint i = 0; i < _material.size(); i++)
	{
		//Name
		uint len;
		file.Read(&len, sizeof(uint));
		String name(len, '\0');
		file.Read(&name[0], len);

		//Type
		Material::Type type;
		file.Read(&type, sizeof(Material::Type));

		if (type == Material::Type::linear)
		{
			//Modulus
			real modulus;
			file.Read(&modulus, sizeof(real));
			_material[i] = new LinearMaterial(name, modulus);
		}
		else
		{
			//Formula
			file.Read(&len, sizeof(uint));
			String formula(len, '\0');
			file.Read(&formula[0], len);
			_material[i] = new NonlinearMaterial(name, formula);
		}
	}
}

void p6::Construction::import(const String filepath)
{
	//Open file
	wxFile file(filepath, wxFile::OpenMode::read);
	if (!file.IsOpened()) throw std::runtime_error("File cannot be opened for read");
	Header header, sample;
	file.Read(&header, sizeof(Header));
	if (memcmp(header.signature, sample.signature, 8) != 0) throw ("Invalid file format");

	uint old_node_size = _node.size();
	uint old_stick_size = _stick.size();
	uint old_force_size = _force.size();
	uint old_material_size = _material.size();

	//Nodes
	_node.resize(old_node_size + header.node);
	for (uint i = old_node_size; i < _node.size(); i++)
	{
		file.Read(&_node[i], sizeof(StaticNode));
	}

	//Sticks
	_stick.resize(old_stick_size + header.stick);
	for (uint i = old_stick_size; i < _stick.size(); i++)
	{
		file.Read(&_stick[i], sizeof(Stick));
		_stick[i].node[0] += old_node_size;
		_stick[i].node[1] += old_node_size;
		_stick[i].material += old_material_size;
	}

	//Forces
	_force.resize(old_force_size + header.force);
	for (uint i = old_force_size; i < _force.size(); i++)
	{
		file.Read(&_force[i], sizeof(Force));
		_force[i].node += old_node_size;
	}

	//Materials
	for (uint i = 0; i < header.material; i++)
	{
		//Name
		uint len;
		file.Read(&len, sizeof(uint));
		String name(len, '\0');
		file.Read(&name[0], len);

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
		file.Read(&type, sizeof(Material::Type));

		//Creating material
		Material *new_material;
		if (type == Material::Type::linear)
		{
			//Modulus
			real modulus;
			file.Read(&modulus, sizeof(real));
			if (existing_material == (uint)-1) new_material = new LinearMaterial(name, modulus);
		}
		else
		{
			//Formula
			file.Read(&len, sizeof(uint));
			String formula(len, '\0');
			file.Read(&formula[0], len);
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

p6::uint p6::Construction::_node_equation_fx(uint free) const noexcept
{
	assert(free < _nfree);
	return 2 * free;
}

p6::uint p6::Construction::_node_equation_fy(uint free) const noexcept
{
	assert(free < _nfree);
	return 2 * free + 1;
}

p6::uint p6::Construction::_equation_number() const noexcept
{
	return 2 * _nfree;
}

p6::uint p6::Construction::_node_variable_x(uint free) const noexcept
{
	assert(free < _nfree);
	return 2 * free;
}

p6::uint p6::Construction::_node_variable_y(uint free) const noexcept
{
	assert(free < _nfree);
	return 2 * free + 1;
}

p6::uint p6::Construction::_variable_number() const noexcept
{
	return 2 * _nfree;
}

void p6::Construction::_check_materials_specified() const
{
	for (uint i = 0; i < _stick.size(); i++)
	{
		assert(_stick[i].material < _material.size() || _stick[i].material == (uint)-1);
		if (_stick[i].material == (uint)-1) throw std::runtime_error("Material is not specified");
	}
}

void p6::Construction::_create_map(std::vector<uint> *node_to_free)
{
	_nfree = 0;
	node_to_free->resize(_node.size(), (uint)-1);
	for (uint i = 0; i < _node.size(); i++)
	{
		if (_node[i].free)
		{
			node_to_free->at(i) = _nfree;
			_nfree++;
		}
	}
}

p6::real p6::Construction::_get_tolerance() const
{
	real minforce = std::numeric_limits<real>::infinity();
	for (uint i = 0; i < _force.size(); i++)
	{
		real newforce = _force[i].direction.modulus();
		if (newforce < minforce) minforce = newforce;
	}
	return minforce / 1000.0;
}

void p6::Construction::_create_vectors(
	const std::vector <uint> *node_to_free,
	Vector *s,
	Vector *z,
	Vector *m,
	Matrix *d) const
{
	s->resize(_variable_number());
	for (uint i = 0; i < _node.size(); i++)
	{
		if (_node[i].free)
		{
			uint free = node_to_free->at(i);
			(*s)(_node_variable_x(free)) = _node[i].coord.x;
			(*s)(_node_variable_y(free)) = _node[i].coord.y;
		}
	}
	z->resize(_equation_number());
	m->resize(_equation_number());
	d->resize(_equation_number(), _variable_number());
}

void p6::Construction::_set_z_to_external_forces(
	const std::vector <uint> *node_to_free,
	Vector *z) const
{
	for (uint i = 0; i < _force.size(); i++)
	{
		uint node = _force[i].node;
		if (_node[node].free)
		{
			uint free = node_to_free->at(node);
			(*z)(_node_variable_x(free)) = _force[i].direction.x;
			(*z)(_node_variable_y(free)) = _force[i].direction.y;
		}
	}
}

void p6::Construction::_set_d_to_zero(
	const std::vector <uint> *node_to_free,
	Matrix *d) const
{
	for (uint i = 0; i < _stick.size(); i++)
	{
		const uint *node = _stick[i].node;
		for (uint j = 0; j < 2; j++)
		{
			if (_node[node[j]].free)
			{
				uint free_j = node_to_free->at(node[j]);
				(*d)(_node_equation_fx(free_j), _node_variable_x(free_j)) = 0.0;
				(*d)(_node_equation_fx(free_j), _node_variable_y(free_j)) = 0.0;
				(*d)(_node_equation_fy(free_j), _node_variable_x(free_j)) = 0.0;
				(*d)(_node_equation_fy(free_j), _node_variable_y(free_j)) = 0.0;
				if (_node[node[j ^ 1]].free)
				{
					uint free_j1 = node_to_free->at(node[j ^ 1]);
					(*d)(_node_equation_fx(free_j), _node_variable_x(free_j1)) = 0.0;
					(*d)(_node_equation_fx(free_j), _node_variable_y(free_j1)) = 0.0;
					(*d)(_node_equation_fy(free_j), _node_variable_x(free_j1)) = 0.0;
					(*d)(_node_equation_fy(free_j), _node_variable_y(free_j1)) = 0.0;
				}
			}
		}
	}
}

p6::Coord p6::Construction::_get_delta(
	uint stick,
	const std::vector <uint> *node_to_free,
	const Vector *s) const
{
	const uint *node = _stick[stick].node;
	Coord coord[2];
	for (uint i = 0; i < 2; i++)
	{
		if (!_node[node[i]].free) coord[i] = _node[node[i]].coord;
		else
		{
			uint free = node_to_free->at(node[i]);
			coord[i] = Coord((*s)(_node_variable_x(free)), (*s)(_node_variable_y(free)));
		}
	}
	return coord[1] - coord[0];
}

void p6::Construction::_modify_z_with_stick_force(
	uint stick,
	const std::vector <uint> *node_to_free,
	const Vector *s,
	Vector *z) const
{
	const Material *material = _material[_stick[stick].material];
	const uint *node = _stick[stick].node;
	Coord delta = _get_delta(stick, node_to_free, s);
	real length = delta.modulus();
	real initial_length = _node[node[0]].coord.distance(_node[node[1]].coord);
	real force = _stick[stick].area * material->stress(length / initial_length - 1.0);

	for (uint i = 0; i < 2; i++)
	{
		if (_node[node[i]].free)
		{
			uint free = node_to_free->at(node[i]);
			real sign = i == 0 ? 1.0 : -1.0;
			(*z)(_node_variable_x(free)) += sign * force * delta.x / length;
			(*z)(_node_variable_y(free)) += sign * force * delta.y / length;
		}
	}
}

void p6::Construction::_modify_d_with_stick_force(
	uint stick,
	const std::vector <uint> *node_to_free,
	const Vector *s,
	Matrix *d) const
{
	const Material *material = _material[_stick[stick].material];
	const uint *node = _stick[stick].node;
	Coord delta = _get_delta(stick, node_to_free, s);
	real length = delta.modulus();
	real initial_length = _node[node[0]].coord.distance(_node[node[1]].coord);
	real strain = length / initial_length - 1.0;

	real dldx0 = -delta.x / length;
	real dfx0dx0 = _stick[stick].area * (
		material->stress(strain) * (-length - dldx0 * delta.x) / sqr(length) +
		material->derivative(strain) * dldx0 * delta.x / (initial_length * length));
	real dfy0dx0 = _stick[stick].area * (
		material->stress(strain) * (-dldx0 * delta.y) / sqr(length) +
		material->derivative(strain) * dldx0 * delta.y / (initial_length * length));
	real dldy0 = -delta.y / length;
	real dfx0dy0 = _stick[stick].area * (
		material->stress(strain) * (-dldy0 * delta.x) / sqr(length) +
		material->derivative(strain) * dldy0 * delta.x / (initial_length * length));
	real dfy0dy0 = _stick[stick].area * (
		material->stress(strain) * (-length - dldy0 * delta.y) / sqr(length) +
		material->derivative(strain) * dldy0 * delta.y / (initial_length * length));
	for (uint j = 0; j < 2; j++)
	{
		if (_node[node[j]].free)
		{
			uint free_j = node_to_free->at(node[j]);
			real sign = j == 0 ? -1.0 : 1.0;
			(*d)(_node_equation_fx(free_j), _node_variable_x(free_j)) += sign * dfx0dx0;
			(*d)(_node_equation_fx(free_j), _node_variable_y(free_j)) += sign * dfx0dy0;
			(*d)(_node_equation_fy(free_j), _node_variable_x(free_j)) += sign * dfy0dx0;
			(*d)(_node_equation_fy(free_j), _node_variable_y(free_j)) += sign * dfy0dy0;
			if (_node[node[j ^ 1]].free)
			{
				uint free_j1 = node_to_free->at(node[j ^ 1]);
				(*d)(_node_equation_fx(free_j), _node_variable_x(free_j1)) -= sign * dfx0dx0;
				(*d)(_node_equation_fx(free_j), _node_variable_y(free_j1)) -= sign * dfx0dy0;
				(*d)(_node_equation_fy(free_j), _node_variable_x(free_j1)) -= sign * dfy0dx0;
				(*d)(_node_equation_fy(free_j), _node_variable_y(free_j1)) -= sign * dfy0dy0;
			}
		}
	}
}

p6::real p6::Construction::_get_error(const Vector *z) const
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
	const Vector *s)
{
	for (uint i = 0; i < _node.size(); i++)
	{
		if (_node[i].free)
		{
			uint free = node_to_free->at(i);
			_node[i].coord_simulated = Coord((*s)(_node_variable_x(free)), (*s)(_node_variable_y(free)));
		}
		else _node[i].coord_simulated = _node[i].coord;
	}
}

bool p6::Construction::_is_adequat(const Vector *m) const
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
	const Vector *z) const
{
	real coef = std::numeric_limits<real>::infinity();
	for (uint i = 0; i < _stick.size(); i++)
	{
		const uint *node = _stick[i].node;
		Coord delta = _get_delta(i, node_to_free, s);
		for (uint j = 0; j < 2; j++)
		{
			if (_node[node[j]].free)
			{
				uint free = node_to_free->at(node[j]);
				real length = delta.modulus();
				real newcoef = length / sqrt(sqr((*z)(_node_equation_fx(free))) + sqr((*z)(_node_equation_fy(free))));
				if (newcoef < coef) coef = newcoef;
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
	Vector s;					//State vector
	Vector z;					//Should-be-zero value
	Vector m;					//Modification of state vector
	Matrix	d;	//Derivative of should-be-zero value
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
		real error = _get_error(&z);
		if (error < tolerance) break;
		else if (error < last_error) not_converge_count = 0;
		else if (++not_converge_count == 1000000) throw (std::runtime_error("Simulation does not converge"));
		s += 0.001 * z;
		/*Eigen::HouseholderQR<Matrix> qr(d);
		m = qr.solve(z);
		if (_is_adequat(&m)) s -= m;
		else s += 0.001 * _get_flow_coefficient(&node_to_free, &s, &z) * z;*/
	}
	_apply_state_vector(&node_to_free, &s);

	_simulation = true;
}

p6::Construction::~Construction()
{
	for (uint i = 0; i < _material.size(); i++) delete _material[i];
}