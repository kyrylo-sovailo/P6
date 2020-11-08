#include "../header/p6_indexer.hpp"

p6::Indexer::Indexer(uint nfree, uint n_stick) noexcept
	: _nfree(nfree), _n_stick(n_stick)
{
}

p6::uint p6::Indexer::node_equation_fx(uint free) const noexcept
{
	return 2 * free;
}

p6::uint p6::Indexer::node_equation_fy(uint free) const noexcept
{
	return 2 * free + 1;
}

p6::uint p6::Indexer::equation_number() const noexcept
{
	return 2 * _nfree;
}

p6::uint p6::Indexer::node_variable_x(uint free) const noexcept
{
	return 2 * free;
}

p6::uint p6::Indexer::node_variable_y(uint free) const noexcept
{
	return 2 * free + 1;
}

p6::uint p6::Indexer::variable_number() const noexcept
{
	return 2 * _nfree;
}