/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_MATERIAL
#define P6_MATERIAL

#include "p6_common.hpp"

namespace p6
{
	///Material. Defines properties of stick
	class Material
	{
	protected:
		String _name;

	public:
		///Type of material
		enum class Type
		{
			linear,
			nonlinear
		};

		const String &name() const;						///< returns name of material
		virtual Type type() const noexcept = 0;			///< Returns type of material
		virtual real stress(real strain) const = 0;		///< Returns stress in dependence of strain
		virtual real derivative(real strain) const = 0;	///< Returns derivative of stress by strain
		virtual ~Material() = 0;						///< Destroys material
	};
}

#endif