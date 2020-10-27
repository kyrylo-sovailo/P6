#include "p6_construction.h"
#include "p6_linear_material.h"
#include "p6_nonlinear_material.h"
#include <wx/file.h>
#include <math.h>
#include <Eigen>

p6::Construction::Construction()
{};

p6::Construction::Construction(const String filepath)
{
	wxFile file(filepath, wxFile::OpenMode::read);
	if (!file.IsOpened()) throw std::runtime_error("File cannot be opened for read");
	Header header, sample;
	file.Read(&header, sizeof(Header));
	if (memcmp(header.signature, sample.signature, 8) != 0) throw ("Invalid file format");
	nodes.resize(header.nnodes);
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		file.Read(&nodes[i], sizeof(Node));
	}
	sticks.resize(header.nsticks);
	for (unsigned int i = 0; i < sticks.size(); i++)
	{
		file.Read(&sticks[i], sizeof(Stick));
	}
	materials.resize(header.nmaterials);
	for (unsigned int i = 0; i < materials.size(); i++)
	{
		unsigned int len;
		file.Read(&len, sizeof(unsigned int));
		String name(len, '\0');
		file.Read(&name[0], len);

		Material::Type type;
		file.Read(&type, sizeof(Material::Type));
		
		if (type == Material::Type::linear)
		{
			real modulus;
			file.Read(&modulus, sizeof(real));
			materials[i] = new LinearMaterial(name, modulus);
		}
		else
		{
			file.Read(&len, sizeof(unsigned int));
			String formula(len, '\0');
			file.Read(&formula[0], len);
			materials[i] = new NonlinearMaterial(name, formula);
		}
	}
};

void p6::Construction::save(const String filepath) const
{
	wxFile file(filepath, wxFile::OpenMode::write);
	if (!file.IsOpened()) throw std::runtime_error("File cannot be opened for reawrited");
	Header header;
	header.nnodes = nodes.size();
	header.nsticks = sticks.size();
	header.nmaterials = materials.size();
	file.Write(&header, sizeof(Header));
	for (unsigned int i = 0; i < nodes.size(); i++)
	{
		file.Write(&nodes[i], sizeof(Node));
	}
	for (unsigned int i = 0; i < sticks.size(); i++)
	{
		file.Write(&sticks[i], sizeof(Stick));
	}
	for (unsigned int i = 0; i < materials.size(); i++)
	{
		String name = ((NonlinearMaterial*)materials[i])->name();
		unsigned int len = name.size();
		file.Write(&len, sizeof(unsigned int));
		file.Write(name.data(), len);
		
		Material::Type type = materials[i]->type();
		file.Write(&type, sizeof(Material::Type));

		if (type == Material::Type::linear)
		{
			real modulus = ((LinearMaterial*)materials[i])->modulus();
			file.Write(&modulus, sizeof(real));
		}
		else
		{
			String formula = ((NonlinearMaterial*)materials[i])->formula();
			len = formula.size();
			file.Write(&len, sizeof(unsigned int));
			file.Write(formula.data(), len);
		}
	}
};

void p6::Construction::merge(Construction *construction)
{
	//Materials
	unsigned int old_materials_number = materials.size();
	for (unsigned int i = 0; i < construction->materials.size(); i++)
	{
		bool found = false;
		for (unsigned int j = 0; j < materials.size(); j++)
		{
			if (materials[j]->name() == construction->materials[i]->name())
			{
				found = true;
				for (unsigned int k = 0; k < construction->sticks.size(); k++)
				{
					if (construction->sticks[k].material_index == i)
						construction->sticks[k].material_index = j;
				}
				delete construction->materials[i];
				break;
			}
		}
		if (!found)
		{
			for (unsigned int k = 0; k < construction->sticks.size(); k++)
			{
				if (construction->sticks[k].material_index == i)
					construction->sticks[k].material_index = materials.size();
			}
			materials.push_back(construction->materials[i]);
		}
	}

	//Sticks
	for (unsigned int i = 0; i < construction->sticks.size(); i++)
	{
		construction->sticks[i].node_index[0] += nodes.size();
		construction->sticks[i].node_index[1] += nodes.size();
	}
	sticks.insert(sticks.cend(), construction->sticks.cbegin(), construction->sticks.cend());


	//Nodes
	nodes.insert(nodes.cend(), construction->nodes.cbegin(), construction->nodes.cend());
};

void p6::Construction::simulate()
{
	//Initial lengths of sticks
	std::vector<real> lengths(sticks.size());
	real flow_coef = std::numeric_limits<real>::infinity();
	for (unsigned int i = 0; i < sticks.size(); i++)
	{
		const unsigned int *node_index = sticks[i].node_index;
		lengths[i] = sqrt(
			sqr(nodes[node_index[0]].x - nodes[node_index[1]].x) +
			sqr(nodes[node_index[0]].y - nodes[node_index[1]].y));
	}

	//Creating general-to-free and free-to general maps
	std::vector<unsigned int> free_to_node;
	std::vector<unsigned int> node_to_free;
	{
		unsigned int nfree = 0;
		node_to_free.resize(nodes.size(), (unsigned int)-1);
		for (unsigned int i = 0; i < nodes.size(); i++)
		{
			if (nodes[i].free)
			{
				node_to_free[i] = nfree;
				nfree++;
			}
		}
		
		free_to_node.reserve(nfree);
		for (unsigned int i = 0; i < nodes.size(); i++)
		{
			if (nodes[i].free)
			{
				free_to_node.push_back(i);
			}
		}
	}

	//Class for generating indexes in vectors/matrixes
	class Indexer
	{
	private:
		unsigned int _nfree, _nsticks;
	public:
		Indexer(unsigned int nfree, unsigned int nsticks) : _nfree(nfree), _nsticks(nsticks) {};
		inline unsigned int node_equation_fx(unsigned int free) { return 2 * free; };
		inline unsigned int node_equation_fy(unsigned int free) { return 2 * free + 1; };
		inline unsigned int equation_number() { return 2 * _nfree; };
		inline unsigned int node_variable_x(unsigned int free) { return 2 * free; };
		inline unsigned int node_variable_y(unsigned int free) { return 2 * free + 1; };
		inline unsigned int variable_number() { return 2 * _nfree; };
	} r(free_to_node.size(), sticks.size());

	//Creating state vector
	Eigen::Vector<real, Eigen::Dynamic> s(r.variable_number());
	for (unsigned int i = 0; i < free_to_node.size(); i++)
	{
		s(r.node_variable_x(i)) = nodes[free_to_node[i]].x;
		s(r.node_variable_y(i)) = nodes[free_to_node[i]].y;
	}

	//Preallocating
	Eigen::Vector<real, Eigen::Dynamic>
		z(r.equation_number());							//Should-be-zero value
	Eigen::Vector<real, Eigen::Dynamic>
		m(r.equation_number());							//Modification
	Eigen::Matrix<real, Eigen::Dynamic, Eigen::Dynamic>
		d(r.equation_number(), r.variable_number());	//Derivative of should-be-zero

	//Calculating tolerance
	real tolerance;
	{
		real minforce = std::numeric_limits<real>::infinity();
		for (unsigned int i = 0; i < nodes.size(); i++)
		{
			if (nodes[i].xforce != 0 || nodes[i].yforce != 0)
			{
				real newforce = sqrt(sqr(nodes[i].xforce) + sqr(nodes[i].yforce));
				if (newforce < minforce) minforce = newforce;
			}
		}
		tolerance = minforce / 1000.0;
	}

	//Iterating
	real last_error = 0.0;
	unsigned int not_converge_count = 0;
	while (true)
	{
		//Calculating should-be-zero and derivative value
		//Setting external forces for free nodes (zeroing should-be-zero)
		for (unsigned int i = 0; i < free_to_node.size(); i++)
		{
			unsigned int node_index = free_to_node[i];
			z(r.node_variable_x(i)) = nodes[node_index].xforce;
			z(r.node_variable_y(i)) = nodes[node_index].yforce;
		}
		//Setting that forces of free nodes do not depend from coordinates of free nodes (zeroing derivative)
		for (unsigned int i = 0; i < sticks.size(); i++)
		{
			const unsigned int *node_index = sticks[i].node_index;
			for (unsigned int j = 0; j < 2; j++)
			{
				if (nodes[node_index[j]].free)
				{
					unsigned int free_index_j = node_to_free[node_index[j]];
					d(r.node_equation_fx(free_index_j), r.node_variable_x(free_index_j)) = 0.0;
					d(r.node_equation_fx(free_index_j), r.node_variable_y(free_index_j)) = 0.0;
					d(r.node_equation_fy(free_index_j), r.node_variable_x(free_index_j)) = 0.0;
					d(r.node_equation_fy(free_index_j), r.node_variable_y(free_index_j)) = 0.0;
					if (nodes[node_index[j^1]].free)
					{
						unsigned int free_index_j1 = node_to_free[node_index[j^1]];
						d(r.node_equation_fx(free_index_j), r.node_variable_x(free_index_j1)) = 0.0;
						d(r.node_equation_fx(free_index_j), r.node_variable_y(free_index_j1)) = 0.0;
						d(r.node_equation_fy(free_index_j), r.node_variable_x(free_index_j1)) = 0.0;
						d(r.node_equation_fy(free_index_j), r.node_variable_y(free_index_j1)) = 0.0;
					}
				}
			}
		}

		//Setting forces and derivatives for pairs of nodes
		for (unsigned int i = 0; i < sticks.size(); i++)
		{
			//Getting coordinates of nodes
			const unsigned int *node_index = sticks[i].node_index;
			real x[2], y[2];
			for (unsigned int j = 0; j < 2; j++)
			{
				if (nodes[node_index[j]].free)
				{
					unsigned int free_index = node_to_free[node_index[j]];
					x[j] = s(r.node_variable_x(free_index));
					y[j] = s(r.node_variable_y(free_index));
				}
				else
				{
					x[j] = nodes[node_index[j]].x;
					y[j] = nodes[node_index[j]].y;
				}
			}
			//Setting should-be-zero
			const Material *material = materials[sticks[i].material_index];
			real dx = x[1] - x[0];
			real dy = y[1] - y[0];
			real length = sqrt(sqr(dx) + sqr(dy));
			real strain = length / lengths[i] - 1.0;
			real force = sticks[i].area * material->stress(strain);
			for (unsigned int j = 0; j < 2; j++)
			{
				if (nodes[node_index[j]].free)
				{
					unsigned int free_index = node_to_free[node_index[j]];
					real sign = j == 0 ? 1.0 : -1.0;
					z(r.node_variable_x(free_index)) += sign * force * dx / length;
					z(r.node_variable_y(free_index)) += sign * force * dy / length;
				}
			}
			//Setting derivative
			real dldx0 = - dx / length;
			real dfx0dx0 = sticks[i].area * (
				material->stress(strain) * (-length - dldx0 * dx) / sqr(length) +
				material->derivative(strain) * dldx0 * dx / (lengths[i] * length)
			);
			real dfy0dx0 = sticks[i].area * (
				material->stress(strain) * (-dldx0 * dy) / sqr(length) +
				material->derivative(strain) * dldx0 * dy / (lengths[i] * length)
			);
			real dldy0 = -dy / length;
			real dfx0dy0 = sticks[i].area * (
				material->stress(strain) * (-dldy0 * dx) / sqr(length) +
				material->derivative(strain) * dldy0 * dx / (lengths[i] * length)
			);
			real dfy0dy0 = sticks[i].area * (
				material->stress(strain) * (-length - dldy0 * dy) / sqr(length) +
				material->derivative(strain) * dldy0 * dy / (lengths[i] * length)
			);
			for (unsigned int j = 0; j < 2; j++)
			{
				if (nodes[node_index[j]].free)
				{
					unsigned int free_index_j = node_to_free[node_index[j]];
					real sign = j == 0 ? -1.0 : 1.0;
					d(r.node_equation_fx(free_index_j), r.node_variable_x(free_index_j)) += sign * dfx0dx0;
					d(r.node_equation_fx(free_index_j), r.node_variable_y(free_index_j)) += sign * dfx0dy0;
					d(r.node_equation_fy(free_index_j), r.node_variable_x(free_index_j)) += sign * dfy0dx0;
					d(r.node_equation_fy(free_index_j), r.node_variable_y(free_index_j)) += sign * dfy0dy0;
					if (nodes[node_index[j ^ 1]].free)
					{
						unsigned int free_index_j1 = node_to_free[node_index[j ^ 1]];
						d(r.node_equation_fx(free_index_j), r.node_variable_x(free_index_j1)) -= sign * dfx0dx0;
						d(r.node_equation_fx(free_index_j), r.node_variable_y(free_index_j1)) -= sign * dfx0dy0;
						d(r.node_equation_fy(free_index_j), r.node_variable_x(free_index_j1)) -= sign * dfy0dx0;
						d(r.node_equation_fy(free_index_j), r.node_variable_y(free_index_j1)) -= sign * dfy0dy0;
					}
				}
			}
		}
		//Normal qutting
		real error = 0.0;
		for (int i = 0; i < z.rows(); i++)
		{
			if (abs(z(i)) > error) error = abs(z(i));
		}
		if (error < tolerance) break;
		else if (error < last_error) not_converge_count = 0;
		else if (++not_converge_count == 1000) break;//throw (std::runtime_error("Simulation does not converge"));

		//Solving
		Eigen::HouseholderQR<Eigen::Matrix<real, Eigen::Dynamic, Eigen::Dynamic>> qr(d);
		m = qr.solve(z);

		//Error handling & "flowing"
		bool flow = false;
		for (int i = 0; i < s.rows(); i++)
		{
			if (m(i) != m(i) || abs(m(i)) == std::numeric_limits<real>::infinity())
			{ flow = true; break; }
		}

		if (!flow) s -= m;
		else
		{
			//Determining of flow coefficient
			real flow_coef = std::numeric_limits<real>::infinity();
			for (unsigned int i = 0; i < sticks.size(); i++)
			{
				const unsigned int *node_index = sticks[i].node_index;
				real x[2], y[2];
				for (unsigned int j = 0; j < 2; j++)
				{
					if (nodes[node_index[j]].free)
					{
						unsigned int free_index = node_to_free[node_index[j]];
						x[j] = s(r.node_variable_x(free_index));
						y[j] = s(r.node_variable_y(free_index));
					}
					else
					{
						x[j] = nodes[node_index[j]].x;
						y[j] = nodes[node_index[j]].y;
					}
				}
				for (unsigned int j = 0; j < 2; j++)
				{
					if (nodes[node_index[j]].free)
					{
						unsigned int free_index = node_to_free[node_index[j]];
						real newcoef = sqrt(sqr(x[1] - x[0]) + sqr(y[1] - y[0])) /
							sqrt(sqr(z(r.node_equation_fx(free_index))) + sqr(z(r.node_equation_fy(free_index))));
						if (newcoef < flow_coef) flow_coef = newcoef;
					}
				}
			}
			s += 0.001 * flow_coef * z;
		}
	}

	//Setting node coordinates from state vector
	for (unsigned int i = 0; i < free_to_node.size(); i++)
	{
		nodes[free_to_node[i]].x = s(r.node_variable_x(i));
		nodes[free_to_node[i]].y = s(r.node_variable_y(i));
	}
};