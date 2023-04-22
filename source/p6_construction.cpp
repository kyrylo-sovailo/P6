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
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <Eigen/SparseLU>

namespace p6
{
	class DenseVector : public Eigen::Matrix<p6::real, Eigen::Dynamic, 1>
	{
	public:
		using Eigen::Matrix<p6::real, Eigen::Dynamic, 1>::Matrix;
	};

	class DenseMatrix : public Eigen::Matrix<p6::real, Eigen::Dynamic, Eigen::Dynamic>
	{
	public:
		using Eigen::Matrix<p6::real, Eigen::Dynamic, Eigen::Dynamic>::Matrix;
	};

	class SparseMatrix : public Eigen::SparseMatrix<p6::real>
	{
	public:
		using Eigen::SparseMatrix<p6::real>::SparseMatrix;
	};

	class TripletVector : public std::vector<Eigen::Triplet<p6::real>>
	{
	public:
		using std::vector<Eigen::Triplet<p6::real>>::vector;
	};
}

p6::uint p6::Construction::create_node() noexcept
{
	assert(!_simulation);
	Node node;
	node.freedom = 0;
	node.coord = Coord(0.0, 0.0);
	node.vector = Coord(1.0, 0.0);
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

void p6::Construction::set_node_rail_vector(uint node, Coord vector) noexcept
{
	assert(!_simulation);
	assert(vector.x == vector.x);
	assert(abs(vector.x) != std::numeric_limits<real>::infinity());
	assert(vector.y == vector.y);
	assert(abs(vector.y) != std::numeric_limits<real>::infinity());
	_node[node].vector = vector;
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

p6::Coord p6::Construction::get_node_rail_vector(uint node) const noexcept
{
	return _node[node].vector;
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
		Material *new_material = nullptr;
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

void p6::Construction::_check_materials_specified() const
{
	for (uint i = 0; i < _stick.size(); i++)
	{
		assert(_stick[i].material < _material.size() || _stick[i].material == (uint)-1);
		if (_stick[i].material == (uint)-1) throw std::runtime_error("Material is not specified");
	}
}

unsigned int p6::Construction::_create_map(std::vector<uint> *map) noexcept
{
	map->resize(_node.size(), (uint)-1);
	unsigned int freedom = 0;
	for (uint i = 0; i < _node.size(); i++)
	{
		if (_node[i].freedom == 1) { map->at(i) = freedom; freedom += 1; }
		else if (_node[i].freedom == 2) { map->at(i) = freedom; freedom += 2; }
	}
	return freedom;
}

p6::real p6::Construction::_find_smallest_force() const noexcept
{
	if (_force.empty()) return 0.0;
	real force = std::numeric_limits<real>::infinity();
	for (uint i = 0; i < _force.size(); i++)
	{
		if (force == 0.0 || force < _force[i].direction.norm()) force = _force[i].direction.norm();
	}
	return force;
}

void p6::Construction::_copy_state() noexcept
{
	for (uint i = 0; i < _node.size(); i++) _node[i].coord_simulated = _node[i].coord;
}

void p6::Construction::_create_state(
	const std::vector<uint> *map,
	DenseVector *state) noexcept
{
	for (uint i = 0; i < _node.size(); i++)
	{
		if (_node[i].freedom == 1)
		{
			(*state)(map->at(i)) = 0.0;
		}
		else if (_node[i].freedom == 2)
		{
			(*state)(map->at(i)    ) = _node[i].coord.x;
			(*state)(map->at(i) + 1) = _node[i].coord.y;
		}
	}
}

void p6::Construction::_apply_state(
	const std::vector<uint> *map,
	const DenseVector *state) noexcept
{
	for (uint i = 0; i < _node.size(); i++)
	{
		if (_node[i].freedom == 1)
		{
			Coord vector = _node[i].vector;
			_node[i].coord_simulated = _node[i].coord + vector * (*state)(map->at(i)) / vector.norm();
		}
		else if (_node[i].freedom == 2)
		{
			_node[i].coord_simulated = Coord((*state)(map->at(i)), (*state)(map->at(i) + 1));
		}
		else _node[i].coord_simulated = _node[i].coord;
	}
}

void p6::Construction::_fill_derivative_and_residual(
	const std::vector<uint> *map,
	const DenseVector *state,
	TripletVector *buffer,
	DenseVector *residual,
	SparseMatrix *derivative) noexcept
{
	//Setting residual  and derivative to zero
	residual->setZero();
	buffer->resize(0);

	//Summing external forces
	for (uint i = 0; i < _force.size(); i++)
	{
		uint node = _force[i].node;
		if (_node[node].freedom == 1)
		{
			Coord rail_vector = _node[node].vector / _node[node].vector.norm();
			(*residual)(map->at(node)) += _force[i].direction.dot(rail_vector);
		}
		else if (_node[node].freedom == 2)
		{
			(*residual)(map->at(node)    ) += _force[i].direction.x;
			(*residual)(map->at(node) + 1) += _force[i].direction.y;
		}
	}

	//Summing self-derivatives and setting other-derivatives
	for (uint i = 0; i < _stick.size(); i++)
	{
		//Calculating essentials
		const uint *node = _stick[i].node;
		Coord delta;
		{
			Coord coord[2];
			for (uint j = 0; j < 2; j++)
			{
				if (_node[node[j]].freedom == 1) coord[j] = _node[node[j]].coord + _node[node[j]].vector * (*state)(map->at(node[j])) / _node[node[j]].vector.norm();
				else if (_node[node[j]].freedom == 1) coord[j] = Coord((*state)(map->at(node[j])), (*state)(map->at(node[j]) + 1));
				else coord[j] = _node[node[j]].coord;
			}
			delta = coord[1] - coord[0];
		}
		const Material *material = _material[_stick[i].material];
		real length = delta.norm();
		real initial_length = (_node[node[0]].coord - _node[node[1]].coord).norm();
		real tension = _stick[i].area * material->stress((length - initial_length) / initial_length);

		//Summing residual
		for (uint j = 0; j < 2; j++)
		{
			Coord stick_vector = delta * ((j == 0) ? 1.0 : -1.0) / length;
			if (_node[node[j]].freedom == 1)
			{
				Coord rail_vector = _node[node[j]].vector / _node[node[j]].vector.norm();
				(*residual)(map->at(node[j])) += tension * stick_vector.dot(rail_vector);
			}
			else if (_node[node[j]].freedom == 2)
			{
				(*residual)(map->at(node[j])    ) += tension * stick_vector.x;
				(*residual)(map->at(node[j]) + 1) += tension * stick_vector.y;
			}
		}

		//Summing/setting derivative
		if (derivative != nullptr) continue;
		real dtension = _stick[i].area * material->derivative((length - initial_length) / initial_length);
		real dl_dx0 = -delta.x / length;
		real dl_dy0 = -delta.y / length;
		real dt_dx0 = dtension * dl_dx0 / initial_length;
		real dt_dy0 = dtension * dl_dy0 / initial_length;
		real dfx0_dx0 = (dt_dx0 * delta.x - tension - dl_dx0 * tension * delta.x) / sqr(length);
		real dfx0_dy0 = delta.x * (dt_dy0 * length - dl_dy0 * tension) / sqr(length);
		real dfy0_dx0 = delta.y * (dt_dx0 * length - dl_dx0 * tension) / sqr(length);
		real dfy0_dy0 = (dt_dy0 * delta.y - tension - dl_dy0 * tension * delta.y) / sqr(length);
		for (uint j = 0; j < 2; j++)
		{
			if (_node[node[j]].freedom == 1) //Current point is fixed on rail
			{
				Coord rail_vector = _node[node[j]].vector / _node[node[j]].vector.norm();
				buffer->push_back(Eigen::Triplet<real>(map->at(node[j]), map->at(node[j]),
				(
					rail_vector.x * (rail_vector.x * dfx0_dx0 + rail_vector.y * dfx0_dy0) +
					rail_vector.y * (rail_vector.x * dfy0_dx0 + rail_vector.y * dfy0_dy0)
				)));
				if (_node[node[j ^ 1]].freedom == 1) //Other point is fixed on rail
				{
					Coord other_rail_vector = _node[node[j ^ 1]].vector / _node[node[j ^ 1]].vector.norm();
					buffer->push_back(Eigen::Triplet<real>(map->at(node[j ^ 1]), map->at(node[j]),
					-(
						rail_vector.x * (other_rail_vector.x * dfx0_dx0 + other_rail_vector.y * dfx0_dy0) +
						rail_vector.y * (other_rail_vector.x * dfy0_dx0 + other_rail_vector.y * dfy0_dy0)
					)));
				}
				else if (_node[node[j ^ 1]].freedom == 2) //Other point is free
				{
					buffer->push_back(Eigen::Triplet<real>(map->at(node[j ^ 1]    ), map->at(node[j]), -(rail_vector.x * dfx0_dx0 + rail_vector.y * dfy0_dx0)));
					buffer->push_back(Eigen::Triplet<real>(map->at(node[j ^ 1] + 1), map->at(node[j]), -(rail_vector.x * dfx0_dy0 + rail_vector.y * dfy0_dy0)));
				}
			}
			else if (_node[node[j]].freedom == 2) //Current point is free
			{
				buffer->push_back(Eigen::Triplet<real>(map->at(node[j]    ), map->at(node[j]    ), dfx0_dx0));
				buffer->push_back(Eigen::Triplet<real>(map->at(node[j]    ), map->at(node[j] + 1), dfx0_dy0));
				buffer->push_back(Eigen::Triplet<real>(map->at(node[j] + 1), map->at(node[j]    ), dfy0_dx0));
				buffer->push_back(Eigen::Triplet<real>(map->at(node[j] + 1), map->at(node[j] + 1), dfy0_dy0));
				if (_node[node[j ^ 1]].freedom == 1) //Other point is fixed on rail
				{
					Coord other_rail_vector = _node[node[j ^ 1]].vector / _node[node[j ^ 1]].vector.norm();
					buffer->push_back(Eigen::Triplet<real>(map->at(node[j]    ), map->at(node[j ^ 1]), -(other_rail_vector.x * dfx0_dx0 + other_rail_vector.y * dfx0_dy0)));
					buffer->push_back(Eigen::Triplet<real>(map->at(node[j] + 1), map->at(node[j ^ 1]), -(other_rail_vector.x * dfy0_dx0 + other_rail_vector.y * dfy0_dy0)));
				}
				else if (_node[node[j ^ 1]].freedom == 2) //Other point is free
				{
					buffer->push_back(Eigen::Triplet<real>(map->at(node[j]    ), map->at(node[j ^ 1]    ), -dfx0_dx0));
					buffer->push_back(Eigen::Triplet<real>(map->at(node[j]    ), map->at(node[j ^ 1] + 1), -dfx0_dy0));
					buffer->push_back(Eigen::Triplet<real>(map->at(node[j] + 1), map->at(node[j ^ 1]    ), -dfy0_dx0));
					buffer->push_back(Eigen::Triplet<real>(map->at(node[j] + 1), map->at(node[j ^ 1] + 1), -dfy0_dy0));
				}
			}
		}	
	}
	if (derivative != nullptr) derivative->setFromTriplets(buffer->begin(), buffer->end());
}

void p6::Construction::_fix_infinite_correction(
	const std::vector<uint> *map,
	const DenseVector *state,
	DenseVector *correction) noexcept
{
	for (uint i = 0; i < _stick.size(); i++)
	{
		//Calculating essentials
		const uint *node = _stick[i].node;
		Coord coord[2];
		for (uint j = 0; j < 2; j++)
		{
			if (_node[node[j]].freedom == 1) coord[j] = _node[node[j]].coord + _node[node[j]].vector * (*state)(map->at(node[j])) / _node[node[j]].vector.norm();
			else if (_node[node[j]].freedom == 1) coord[j] = Coord((*state)(map->at(node[j])), (*state)(map->at(node[j]) + 1));
			else coord[j] = _node[node[j]].coord;
		}
		real length = (coord[1] - coord[0]).norm();

		//Limiting corrections
		const real limiter = 0.1 * length;
		for (uint j = 0; j < 2; j++)
		{
			if (_node[node[j]].freedom == 1)
			{
				if ((*correction)(map->at(node[j])) > limiter) (*correction)(map->at(node[j])) = limiter;
			}
			else if (_node[node[j]].freedom == 2)
			{
				if ((*correction)(map->at(node[j])    ) > limiter) (*correction)(map->at(node[j])    ) = limiter;
				if ((*correction)(map->at(node[j]) + 1) > limiter) (*correction)(map->at(node[j]) + 1) = limiter;
			}
		}
	}
}

void p6::Construction::simulate(bool sim)
{
	if (sim == _simulation) return;
	else if (!sim) { _simulation = false; return; }

	//Checking if materials are specified
	_check_materials_specified();

	//Find smallest force
	real smallest_force = _find_smallest_force();
	if (smallest_force == 0.0) { _copy_state(); _simulation = false; return; }

	//Creating node-to-free map
	std::vector<uint> map;
	unsigned int freedom = _create_map(&map);

	//Declare variables
	TripletVector buffer;
	DenseVector state(freedom), forward_state(freedom), correction(freedom), residual(freedom);
	SparseMatrix derivative;
	Eigen::SparseLU<SparseMatrix, Eigen::COLAMDOrdering<int>> solver;
	_create_state(&map, &state);
	real max_residual = std::numeric_limits<real>::infinity();
	unsigned int step_divider = 0;
	bool finished = false;
	while (!finished)
	{
		_fill_derivative_and_residual(&map, &state, &buffer, &residual, &derivative);
		solver.analyzePattern(derivative);
		solver.factorize(derivative);
		correction = solver.solve(residual);
		_fix_infinite_correction(&map, &state, &correction);
		if (step_divider > 0) step_divider--;
		while (true)
		{
			forward_state = state - pow(0.5, step_divider) * correction;
			if (forward_state == state) { finished = true; break; }
			_fill_derivative_and_residual(&map, &state, &buffer, &residual, nullptr);
			real new_residual = residual.array().abs().maxCoeff();
			if (new_residual < max_residual) { max_residual = new_residual; state = forward_state; break; }
			else step_divider++;
		}
	}
	if (max_residual < _find_smallest_force() / 1000.0)
	{
		_apply_state(&map, &state);
		_simulation = true;
	}
	else
	{
		throw std::runtime_error("Simulation does not converge");
	}
}

p6::Construction::~Construction()
{
	for (uint i = 0; i < _material.size(); i++) delete _material[i];
}