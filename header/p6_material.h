#ifndef P6_MATERIAL
#define P6_MATERIAL

#include "p6_common.h"

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
};

#endif