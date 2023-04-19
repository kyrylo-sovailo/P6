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
	///Truss construction
	class Construction
	{
	private:
		class Vector;	///<Mathematical vector
		class Matrix;	///<Mathematical matrix

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

		void _check_materials_specified()					const;			///<Checks if materials of all sticks are specified
		unsigned int _create_map(std::vector<uint> *map)	noexcept;		///<Creates node -> equation/variable map, returns degree of freedom
		real _get_tolerance()								const noexcept;	///<Returns force tolerance
		
		///Creates state, should-be-zero, modification vectors and derivative matrix
		void _create_vectors(
			unsigned int freedom,
			const std::vector <uint> *map,
			Vector *s,
			Vector *z,
			Vector *m,
			Matrix *d) const noexcept;
		///Sets should-be-zero value to external forces
		void _set_z_to_external_forces(
			const std::vector <uint> *map,
			Vector *z) const noexcept;
		///Sets derivative of should-be-zero to zero
		void _set_d_to_zero(
			const std::vector <uint> *map,
			Matrix *d) const noexcept;
		///Gets coordinate difference between two nodes
		Coord _get_delta(
			uint stick,
			const std::vector <uint> *map,
			const Vector *s) const noexcept;
		///Modifies should-be-zero value with force of some stick
		void _modify_z_with_stick_force(
			uint stick,
			const std::vector <uint> *map,
			const Vector *s,
			Vector *z) const noexcept;
		///Modifies derivative of should-be-zero with derivatives of force of some stick
		void _modify_d_with_stick_force(
			uint stick,
			const std::vector <uint> *map,
			const Vector *s,
			Matrix *d) const noexcept;	
		///Gets residuum
		real _get_residuum(const Vector *z) const noexcept;
		///Decides if Newton's modification is adequate
		bool _is_adequate(
			const Vector *m,
			const std::vector <uint> *map,
			const Vector *s) const noexcept;
		///Gets flow coefficient
		real _get_flow_coefficient(
			const std::vector <uint> *map,
			const Vector *s,
			const Vector *z) const noexcept;
		///Sets items' data correspondent to state vector
		void _apply_state_vector(
			const std::vector<uint> *map,
			const Vector *s) noexcept;

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
