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
#include <limits>

p6::LinearMaterial::LinearMaterial(const String name, real modulus)
{
	if (name == "") throw std::runtime_error("Material name can not be empty");
	if (modulus != modulus)
		throw std::runtime_error("Young's modulus can not be NaN");
	if (modulus == std::numeric_limits<real>::infinity())
		throw std::runtime_error("Young's modulus can not be infinity");
	if (modulus <= 0.0)
		throw std::runtime_error("Young's modulus can not be less or equal zero");
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
