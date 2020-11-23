/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	d by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_construction.hpp"
#include "../header/p6_linear_material.hpp"
#include "../header/p6_nonlinear_material.hpp"
#include <gtest/gtest.h>
#include <limits>
#include <cmath>

//Linear material test
TEST(LinearMaterial, NegativeModule)
{
	EXPECT_ANY_THROW(p6::LinearMaterial("name", -1.0));
}

TEST(LinearMaterial, NullModule)
{
	EXPECT_ANY_THROW(p6::LinearMaterial("name", 0.0));
}

TEST(LinearMaterial, InfiniteModule)
{
	EXPECT_ANY_THROW(p6::LinearMaterial("name", std::numeric_limits<p6::real>::infinity()));
}

TEST(LinearMaterial, NaNModule)
{
	EXPECT_ANY_THROW(p6::LinearMaterial("name", nan("")));
}

TEST(LinearMaterial, NoName)
{
	EXPECT_ANY_THROW(p6::LinearMaterial("", 1.0));
}

TEST(LinearMaterial, GoodModule)
{	
	EXPECT_EQ(p6::LinearMaterial("name", 2.0).stress(0.001), 0.002);
}

//Nonlinear materiaä test
TEST(NonlinearMaterial, InfinityInFormula)
{
	EXPECT_ANY_THROW(p6::NonlinearMaterial("name", "-s * s + s / 2 - inf"));
}

TEST(NonlinearMaterial, NaNInFormula)
{
	EXPECT_ANY_THROW(p6::NonlinearMaterial("name", "-s * s + s / 2 - nan"));
}

TEST(NonlinearMaterial, BracketErrorInFormula)
{
	EXPECT_ANY_THROW(p6::NonlinearMaterial("name", "sin(x))"));
}

TEST(NonlinearMaterial, IllegalSymbolInFormula)
{
	EXPECT_ANY_THROW(p6::NonlinearMaterial("name", "sin(#)"));
}

TEST(NonlinearMaterial, TwoNumbersInFormula)
{
	EXPECT_ANY_THROW(p6::NonlinearMaterial("name", "ln(s) ln(s)"));
}

TEST(NonlinearMaterial, NoFormula)
{
	EXPECT_ANY_THROW(p6::NonlinearMaterial("name", " "));
}

TEST(NonlinearMaterial, NoName)
{
	EXPECT_ANY_THROW(p6::NonlinearMaterial material("", "sin(s)"));
}

TEST(NonlinearMaterial, GoodFormula1)
{
	EXPECT_EQ(p6::NonlinearMaterial("name", "sin(cos(ln(s)))").stress(3.0), sin(cos(log(3.0))));
	EXPECT_EQ(p6::NonlinearMaterial("name", "sin(cos(ln(s)))").derivative(3.0), cos(cos(log(3.0))) * (-sin(log(3.0))) * (1.0 / 3.0));
}

TEST(NonlinearMaterial, GoodFormula2)
{
	EXPECT_EQ(p6::NonlinearMaterial("name", "-s * s + s / 2 - 1").stress(3.0), -8.5);
	EXPECT_EQ(p6::NonlinearMaterial("name", "-s * s + s / 2 - 1").derivative(3.0), -5.5);
}

//Construction
TEST(Construction, LinearCalculation)
{
	p6::Construction con;
	con.create_node(p6::Coord(-1.0, 0.0), false);
	con.create_node(p6::Coord(1.0, 0.0), false);
	con.create_node(p6::Coord(0.0, 1.0), true);
	con.create_linear_material("steel", 100.0);
	p6::uint stick[2]; stick[0] = 0; stick[1] = 2;
	con.create_stick(stick, 0, 1.0);
	stick[0] = 1; stick[1] = 2;
	con.create_stick(stick, 0, 1.0);
	con.create_force(2, p6::Coord(1.0, 0.0));
	con.simulate(true);
	EXPECT_NEAR(con.get_node_coord(2).x, 0.0141302, 0.0001);
	EXPECT_NEAR(con.get_node_coord(2).y, 1.00005, 0.0001);
}

TEST(Construction, NonlinearCalculation)
{
	p6::Construction con;
	con.create_node(p6::Coord(-1.0, 0.0), false);
	con.create_node(p6::Coord(1.0, 0.0), false);
	con.create_node(p6::Coord(0.0, 1.0), true);
	con.create_nonlinear_material("goo", "s * s * s * 100");
	p6::uint stick[2]; stick[0] = 0; stick[1] = 2;
	con.create_stick(stick, 0, 1.0);
	stick[0] = 1; stick[1] = 2;
	con.create_stick(stick, 0, 1.0);
	con.create_force(2, p6::Coord(1.0, 0.0));
	con.simulate(true);
	EXPECT_NEAR(con.get_node_coord(2).x, 0.388449, 0.0001);
	EXPECT_NEAR(con.get_node_coord(2).y, 0.985997, 0.0001);
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
}
