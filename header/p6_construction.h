#ifndef P6_CONSTRUCTION
#define P6_CONSTRUCTION

#include "p6_node.h"
#include "p6_stick.h"
#include "p6_material.h"
#include <vector>

namespace p6
{
	///Construction. Contains nodes, sticks and materials 
	struct Construction
	{
	private:
		struct Header
		{
			char signature[8] = { 'P','6', 'C', 'N', 'S', 'T', '0', '\0'};
			unsigned int nmaterials;
			unsigned int nnodes;
			unsigned int nsticks;
		};

	public:
		std::vector<Node> nodes;					///< List of nodes of construction
		std::vector<Stick> sticks;					///< List of sticks of construction
		std::vector<Material*> materials;			///< List of materials of construction
		Construction();								///< Creates new constuction
		Construction(const String filepath);		///< Loads construction from file
		void save(const String filepath) const;	///< Saves construction to file
		void merge(Construction *construction);		///< Merges construction with another one
		void simulate();							///< Performs simulation of construction
	};
};

#endif	//#ifndef P6