/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_INDEXER
#define P6_INDEXER

#include "p6_common.hpp"

namespace p6
{
	class Indexer
	{
	private:
		uint _nfree, _n_stick;

	public:
		Indexer(uint nfree, uint n_stick)	noexcept;
		uint node_equation_fx(uint free)	const noexcept;
		uint node_equation_fy(uint free)	const noexcept;
		uint equation_number()				const noexcept;
		uint node_variable_x(uint free)		const noexcept;
		uint node_variable_y(uint free)		const noexcept;
		uint variable_number()				const noexcept;
	};
}

#endif