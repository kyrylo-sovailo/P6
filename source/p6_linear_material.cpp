/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_linear_material.hpp"
#include <stdexcept>

p6::LinearMaterial::LinearMaterial(const String name, real modulus)
{
	if (name == "") throw std::runtime_error("Name of material cannot be empty");
	if (!(modulus > 0.0)) throw std::runtime_error("Young's modulus must be greater then zero");
	_name = name;
	_modulus = modulus;
}

p6::real p6::LinearMaterial::modulus() const noexcept
{
	return _modulus;
}

p6::Material::Type p6::LinearMaterial::type() const noexcept
{
	return p6::Material::Type::linear;
}

p6::real p6::LinearMaterial::stress(real strain) const noexcept
{
	return _modulus * strain;
}

p6::real p6::LinearMaterial::derivative(real strain) const noexcept
{
	return _modulus;
}
