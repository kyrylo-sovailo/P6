#ifndef P6_LINEAR_MATERIAL
#define P6_LINEAR_MATERIAL

#include "p6_common.h"
#include "p6_material.h"

namespace p6
{
	///Material with constant Young's modulus
	class LinearMaterial : public Material
	{
	private:
		real _modulus;										///< Young's modulus

	public:
		LinearMaterial(const String &name, real modulus);	///< Creates material from Young's modulus
		real modulus() const noexcept;						///< Returns Young's modulus
		virtual Type type() const noexcept;					///< Returns type of material
		virtual real stress(real strain) const;				///< Returns stress in dependence of strain
		virtual real derivative(real strain) const;			///< Returns derivative of stress by strain
	};
};

#endif