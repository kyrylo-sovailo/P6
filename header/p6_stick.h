#ifndef P6_STICK
#define P6_STICK

#include "p6_common.h"

namespace p6
{
	///Stick. Basic element that describes relations between points
	struct Stick
	{
		unsigned int node_index[2];	///< Index of connected nodes in Construction::nodes
		unsigned int material_index;///< Index of stick material
		real area;					///< Cross-sectional area of stick
	};
};

#endif