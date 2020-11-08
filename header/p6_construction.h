#ifndef P6_CONSTRUCTION
#define P6_CONSTRUCTION

#include "p6_material.h"
#include <vector>

namespace p6
{
	struct Construction
	{
	private:
		struct Header
		{
			char signature[8] = { 'P','6', 'C', 'N', 'S', 'T', '0', '\0'};
			unsigned int node;
			unsigned int stick;
			unsigned int force;
			unsigned int material;
		};

		struct StaticNode
		{
			bool free;
			real x;
			real y;
		};

		struct Node : StaticNode
		{
			real x_simulated;
			real y_simulated;
		};

		struct Stick
		{
			size_t node[2];
			size_t material;
			real area;
		};

		struct Force
		{
			size_t node;
			real x;
			real y;
		};

		std::vector<Node> _node;
		std::vector<Stick> _stick;
		std::vector<Force> _force;
		std::vector<Material*> _material;
		bool _simulation;

	public:
		//Node
		size_t create_node(real x, real y, bool free);
		void delete_node(size_t node);
		void set_node_x(size_t node, real x);
		void set_node_y(size_t node, real y);
		void set_node_free(size_t node, bool free);
		size_t get_node_count()								const;
		real get_node_x(size_t node)						const;
		real get_node_y(size_t node)						const;
		bool get_node_free(size_t node)						const;
		
		//Stick
		size_t create_stick(const size_t node[2], size_t material, real area);
		void delete_stick(size_t stick);
		void set_stick_material(size_t stick, size_t material);
		void set_stick_area(size_t stick, real area);
		size_t get_stick_count()							const;
		size_t get_stick_material(size_t stick)				const;
		real get_stick_area(size_t stick)					const;
		void get_stick_node(size_t stick, size_t node[2])	const;
		real get_stick_length(size_t stick)					const;
		real get_stick_strain(size_t stick)					const;
		real get_stick_force(size_t stick)					const;

		//Force
		size_t create_force(size_t node, real x, real y);
		void delete_force(size_t force);
		void set_force_x(size_t force, real x);
		void set_force_y(size_t force, real y);
		size_t get_force_count()							const;
		real get_force_x(size_t force)						const;
		real get_force_y(size_t force)						const;
		size_t get_force_node(size_t force)					const;

		//Material
		size_t create_linear_material(const String &name, real modulus);
		size_t create_nonlinear_material(const String &name, const String &formula);
		void delete_material(size_t material);
		size_t get_material_count()							const;
		String get_material_name(size_t material)			const;
		Material::Type get_material_type(size_t material)	const;
		real get_material_modulus(size_t material)			const;
		String get_material_formula(size_t material)		const;
		
		//Maintanance
		void save(const String &filepath)					const;
		void load(const String &filepath);
		void import(const String &filepath);
		void simulate(bool sim);
		bool simulation()									const;
		~Construction();
	};
};

#endif	//#ifndef P6