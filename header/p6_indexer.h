#ifndef P6_INDEXER
#define P6_INDEXER

namespace p6
{
	class Indexer
	{
	private:
		size_t _nfree, _n_stick;

	public:
		Indexer(size_t nfree, size_t n_stick) : _nfree(nfree), _n_stick(n_stick) {};
		inline size_t node_equation_fx(size_t free) { return 2 * free; };
		inline size_t node_equation_fy(size_t free) { return 2 * free + 1; };
		inline size_t equation_number() { return 2 * _nfree; };
		inline size_t node_variable_x(size_t free) { return 2 * free; };
		inline size_t node_variable_y(size_t free) { return 2 * free + 1; };
		inline size_t variable_number() { return 2 * _nfree; };
	};
};

#endif