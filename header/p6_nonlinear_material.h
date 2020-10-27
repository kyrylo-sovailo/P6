#ifndef P6_NONLINEAR_MATERIAL
#define P6_NONLINEAR_MATERIAL

#include "p6_common.h"
#include "p6_material.h"
#include <vector>

namespace p6
{
	///Material with arbitrary dependence of stress from strain
	class NonlinearMaterial : public Material
	{
	private:
		enum class OperationCode : unsigned char
		{
			PUTR,
			PUTS,
			ADD,
			SUB,
			MUL,
			DIV,
			NEG,
			SIN,
			COS,
			LN,
			EXP
		};

		mutable real _last_strain;										///< Last given strain
		mutable real _last_stress;										///< Stress from last given strain
		mutable real _last_derivative;									///< Derivative from last given strain
		
		String _formula;												///< Formula of stress in dependence of strain
		std::vector<OperationCode> _opcodes;							///< Translated bytecode of the formula
		void _calculate() const;										///< Calculates stress and derivative from strain

	public:
		NonlinearMaterial(const String &name, const String &formula);	///< Creates material from stress from strain formula
		const String &formula();										///< Returns constant reference to formula

		virtual Type type() const noexcept;								///< Returns type of material
		virtual real stress(real strain) const;							///< Returns stress in dependence of strain
		virtual real derivative(real strain) const;						///< Returns derivative of stress by strain
	};
};

#endif