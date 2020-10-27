#include "p6_linear_material.h"
#include <stdexcept>

p6::LinearMaterial::LinearMaterial(const String &name, real modulus)
{
	if (name == "") throw std::runtime_error("Name of material cannot be empty");
	if (!(modulus > 0.0)) throw std::runtime_error("Young's modulus must be greater then zero");
	_name = name;
	_modulus = modulus;
};

p6::real p6::LinearMaterial::modulus() const noexcept
{
	return _modulus;
};

p6::Material::Type p6::LinearMaterial::type() const noexcept
{
	return p6::Material::Type::linear;
};

p6::real p6::LinearMaterial::stress(real strain) const
{
	return _modulus * strain;
};

p6::real p6::LinearMaterial::derivative(real strain) const
{
	return _modulus;
};
