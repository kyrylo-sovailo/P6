#ifndef P6_NODE
#define P6_NODE

#include "p6_common.h"

namespace p6
{
	///Node. Basic element that describes coordinates of points. There can be a stick between each two nodes
	struct Node
	{
		real x;		///< Horizontal coordinate of node [Set by Construction::simulate() for free nodes]
		real y;		///< Vertical coordinate of node [Changed by Construction::simulate() for free nodes]
		real xforce;///< Horizontal force on the node [Set by Construction::simulate() for fixed nodes]
		real yforce;///< Vertical force on the node [Set by Construction::simulate() for fixed nodes]
		bool free;	///< Says if point is free and can be moved
	};
};

#endif