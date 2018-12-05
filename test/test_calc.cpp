#include "calc.h"

#include <gtest.h>


TEST(test_calc, preprocessor_constructor)
{
	preprocessor a = preprocessor("input", functions("input"));
	EXPECT_EQ("input", a.get_functions().input);
}
TEST(test_calc, preprocessor_get_functions)
{
	preprocessor a = preprocessor("input", functions("functions"));
	EXPECT_EQ("functions", a.get_functions().input);
}
TEST(test_calc, preprocessor_get_postfix)
{
	preprocessor a = preprocessor("3+2");
	EXPECT_EQ(3, a.get_postfix().pop().d_value);
}
TEST(test_calc, preprocessor_get_prefix)
{
	preprocessor a = preprocessor("3+2");
	EXPECT_EQ(3, a.get_prefix().pop().d_value);
}
TEST(test_calc, calc_constructor)
{
	calc a = calc("input");
	EXPECT_EQ("input", a.input);
}
TEST(test_calc, calc_plus)
{
	calc a = calc("3+2");
	EXPECT_EQ(5, a.get_result());
}
TEST(test_calc, calc_minus)
{
	calc a = calc("3-2");
	EXPECT_EQ(1, a.get_result());
}
TEST(test_calc, calc_multiply)
{
	calc a = calc("3*2");
	EXPECT_EQ(6, a.get_result());
}
TEST(test_calc, calc_divide)
{
	calc a = calc("4/2");
	EXPECT_EQ(2, a.get_result());
}
TEST(test_calc, calc_plus_multiply)
{
	calc a = calc("3+4*2");
	EXPECT_EQ(11, a.get_result());
}
TEST(test_calc, calc_minus_divide)
{
	calc a = calc("3-4/2");
	EXPECT_EQ(1, a.get_result());
}
TEST(test_calc, calc_plus_multiply_brackets)
{
	calc a = calc("(6+4)*2");
	EXPECT_EQ(20, a.get_result());
}
TEST(test_calc, calc_minus_divide_brackets)
{
	calc a = calc("(6-4)/2");
	EXPECT_EQ(1, a.get_result());
}
TEST(test_calc, calc_sin)
{
	calc a = calc("sin(1.5)");
	double b = sin(1.5);
	EXPECT_EQ(b, a.get_result());
}
TEST(test_calc, calc_sin)
{
	calc a = calc("sin(1.5)");
	double b = sin(1.5);
	EXPECT_EQ(b, a.get_result());
}