/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_NONLINEAR_MATERIAL
#define P6_NONLINEAR_MATERIAL

#include "p6_common.hpp"
#include "p6_material.hpp"
#include <vector>

namespace p6
{
	///Material with arbitrary dependence of stress from strain
	class NonlinearMaterial : public Material
	{
	private:
		///Basic command for executing
		enum class Operation : unsigned char
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

		///Structure representing one word from user input
		struct Word
		{
			///Type of word, can be statically casted into Operation
			enum class Type : unsigned char
			{
				REAL,
				STRAIN,
				ADD,
				SUB,
				MUL,
				DIV,
				NEG,
				SIN,
				COS,
				LN,
				EXP,
				OPEN,
				CLOSE
			};

			Type type;
			real number;
		};

		///Stack element used for calculation
		struct StackElement
		{
			real value;
			real derivative;
		};

		mutable real _last_strain;										///< Last given strain
		mutable real _last_stress;										///< Stress from last given strain
		mutable real _last_derivative;									///< Derivative from last given strain
		
		String _formula;												///< Formula of stress in dependence of strain
		std::vector<Operation> _operations;								///< Translated bytecode of the formula
		void _calculate() const;										///< Calculates stress and derivative from strain

	public:
		NonlinearMaterial(const String &name, const String &formula);	///< Creates material from stress from strain formula
		const String &formula();										///< Returns constant reference to formula

		virtual Type type() const noexcept;								///< Returns type of material
		virtual real stress(real strain) const;							///< Returns stress in dependence of strain
		virtual real derivative(real strain) const;						///< Returns derivative of stress by strain
	};
}

#endif