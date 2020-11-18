/*
	This software is distributed under MIT License, which means:
		- Do whatever you want
		- Please keep this notice and include the license file to your project
		- I provide no warranty

	Created by Kyrylo Sovailo (github.com/Meta-chan, k.sovailo@gmail.com)
	Reinventing bicycles since 2020
*/

#include "../header/p6_construction.hpp"
#include <gtest/gtest.h>
#include <cmath>

TEST(SqrtTest, PositiveTest)
{
	EXPECT_EQ(1.0, sqrt(1.0));
}

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
}