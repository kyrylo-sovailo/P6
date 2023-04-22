/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#ifndef P6_CONSTRUCTION
#define P6_CONSTRUCTION

#include "p6_material.hpp"
#include <vector>

namespace p6
{
	class DenseVector;	///<Dense vector
	class DenseMatrix;	///<Dense matrix
	class SparseMatrix;	///<Sparse matrix
	class TripletVector;///<Vector of triplets

	///Truss construction
	class Construction
	{
		///File header
		struct Header
		{
			char signature[8] = { 'P','6', 'C', 'N', 'S', 'T', '0', '\0'};
			uint node;
			uint stick;
			uint force;
			uint material;
		};

		///Node data valid both during editing and simulation
		struct StaticNode
		{
			unsigned char freedom;
			Coord coord;
			Coord vector;
		};

		///Node data valid during simulation only
		struct Node : StaticNode
		{
			Coord coord_simulated;
		};

		///Stick data
		struct Stick
		{
			uint node[2];
			uint material;
			real area;
		};
		
		///Force data
		struct Force
		{
			uint node;
			Coord direction;
		};

		std::vector<Node> _node;			///<List of all nodes
		std::vector<Stick> _stick;			///<List of all sticks
		std::vector<Force> _force;			///<List of all forces
		std::vector<Material*> _material;	///<List of all materials
		bool _simulation = false;			///<Indicator if simulation is being run

		///Checks if materials of all sticks are specified
		void _check_materials_specified() const;
		///Creates node -> equation/variable map, returns degree of freedom
		unsigned int _create_map(std::vector<uint> *map) noexcept;
		///Finds smallest external force
		real _find_smallest_force() const noexcept;
		///Copies coordinates from coord to simulated_coord
		void _copy_state() noexcept;
		///Creates state and fills with initial values
		void _create_state(const std::vector<uint> *map, DenseVector *state) noexcept;
		///Read state and write simulated coordinates
		void _apply_state(const std::vector<uint> *map, const DenseVector *state) noexcept;
		///Fills derivative and residual with values
		void _fill_derivative_and_residual(const std::vector<uint> *map, const DenseVector *state, TripletVector *buffer, DenseVector *residual, SparseMatrix *derivative) noexcept;
		///Limit corrections with fraction of stick's length
		void _fix_infinite_correction(const std::vector<uint> *map, const DenseVector *state, DenseVector *correction) noexcept;

	public:
		//Node
		uint create_node()										noexcept;		///<Creates node, returns it's index
		void delete_node(uint node)								noexcept;		///<Deletes node
		void set_node_coord(uint node, Coord coord)				noexcept;		///<Sets node's coordinate
		void set_node_freedom(uint node, unsigned char freedom)	noexcept;		///<Sets node's degree of freedom
		void set_node_rail_vector(uint node, Coord vector)		noexcept;		///<Sets node's rail vector (for nodes fixed on rail)
		uint get_node_count()									const noexcept;	///<Returns node number
		Coord get_node_coord(uint node)							const noexcept;	///<Returns node's coordinates
		unsigned char get_node_freedom(uint node)				const noexcept;	///<Returns if node's degree of freedom
		Coord get_node_rail_vector(uint node)					const noexcept;	///<Returns node's rail vector (for nodes fixed on rail)
		
		//Stick
		uint create_stick(const uint node[2])					noexcept;		///<Creates stick or finds existing one, returns it's index
		void delete_stick(uint stick)							noexcept;		///<Deletes stick
		void set_stick_material(uint stick, uint material)		noexcept;		///<Sets stick's material
		void set_stick_area(uint stick, real area)				noexcept;		///<Sets stick's cross-sectional area
		uint get_stick_count()									const noexcept;	///<Returns stick number
		uint get_stick_material(uint stick)						const noexcept;	///<Returns stick's material
		real get_stick_area(uint stick)							const noexcept;	///<Returns stick's cross-sectional area
		void get_stick_node(uint stick, uint node[2])			const noexcept;	///<Returns nodes stick is attached to
		real get_stick_length(uint stick)						const noexcept;	///<Returns stick's length
		real get_stick_strain(uint stick)						const noexcept;	///<Returns stick's strain
		real get_stick_force(uint stick)						const noexcept;	///<Returns stick's force

		//Force
		uint create_force(uint node)						noexcept;		///<Creates force, returns it's index
		void delete_force(uint force)						noexcept;		///<Deltes force
		void set_force_direction(uint force, Coord coord)	noexcept;		///<Sets force's direction
		uint get_force_count()								const noexcept;	///<Returns force number
		Coord get_force_direction(uint force)				const noexcept;	///<Returns force's direction
		uint get_force_node(uint force)						const noexcept;	///<Returns node force is attached to

		//Material
		uint create_linear_material(const String name, real modulus);					///<Creates linear material, returns it's index
		uint create_nonlinear_material(const String name, const String formula);		///<Creates non-linear material, returns it's index
		void delete_material(uint material)								noexcept;		///<Deletes material
		uint get_material_count()										const noexcept;	///<Returns material number
		String get_material_name(uint material)							const noexcept;	///<Returns material's name
		Material::Type get_material_type(uint material)					const noexcept;	///<Returns material's type
		real get_material_modulus(uint material)						const noexcept;	///<Returns linear material's Young's modulus
		String get_material_formula(uint material)						const noexcept;	///<Returns non-linear material's stress-srain formula
		
		//Maintanance
		void save(const String filepath) const;	///<Saves construction to file
		void load(const String filepath);		///<Loads constuction from file
		void import(const String filepath);		///<Imports consruction from file
		void simulate(bool sim);				///<Runs or inverts simulation

		~Construction();						///<Destroys construction
	};
}

#endif	//#ifndef P6
