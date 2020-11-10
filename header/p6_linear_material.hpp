/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_LINEAR_MATERIAL
#define P6_LINEAR_MATERIAL

#include "p6_common.hpp"
#include "p6_material.hpp"

namespace p6
{
	///Material with constant Young's modulus
	class LinearMaterial : public Material
	{
	private:
		real _modulus;										///< Young's modulus

	public:
		LinearMaterial(const String name, real modulus);	///< Creates material from Young's modulus
		real modulus() const noexcept;						///< Returns Young's modulus
		virtual Type type() const noexcept;					///< Returns type of material
		virtual real stress(real strain) const;				///< Returns stress in dependence of strain
		virtual real derivative(real strain) const;			///< Returns derivative of stress by strain
	};
}

#endif