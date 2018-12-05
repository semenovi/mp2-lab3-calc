#include "container.h"

#include <gtest.h>


TEST(test_container, lexem_default_constructor)
{
	lexem a = lexem();
	EXPECT_EQ(0, a.type);
}
TEST(test_container, lexem_int_constructor)
{
	lexem a = lexem(9);
	EXPECT_EQ(0, a.type);
}
TEST(test_container, lexem_string_constructor)
{
	lexem a = lexem("a");
	EXPECT_EQ(1, a.type);
}
TEST(test_container, lexem_priority)
{
	lexem a = lexem("+");
	EXPECT_EQ(1, a.priority());
}
TEST(test_container, lexem_incorrect_priority)
{
	lexem a = lexem("a");
	EXPECT_EQ(-1, a.priority());
}
TEST(test_container, lexem_not_equal)
{
	lexem a = lexem("a");
	lexem b = lexem("b");
	EXPECT_NE(a, b);
}
TEST(test_container, lexem_equal)
{
	lexem a = lexem("a");
	lexem b = a;
	EXPECT_EQ(a.type, b.type);
}
TEST(test_container, functions_default_constructor)
{
	functions a = functions();
	EXPECT_EQ("", a.input);
}
TEST(test_container, functions_string_constructor)
{
	functions a = functions("a+b");
	EXPECT_EQ("a+b", a.input);
}
TEST(test_container, functions_add)
{
	functions a = functions();
	a.add_function("name", "body");
	EXPECT_EQ("name", a.name[0]);
}
TEST(test_container, functions_equal)
{
	functions a = functions();
	a.add_function("name", "body");
	functions b = a;
	EXPECT_EQ(a.name[0], b.name[0]);
}
TEST(test_container, state_string_constructor)
{
	state a = state("input");
	EXPECT_EQ("input", a.input);
}
TEST(test_container, state_constructor)
{
	lexem vars[50];
	lexem b = lexem("lexem");
	vars[0] = b;
	state a = state("input", vars, 1);
	EXPECT_EQ("lexem", a.vars[0].s_value);
}
TEST(test_container, stack_constructor)
{
	stack a = stack(2);
	EXPECT_EQ(true, a.is_empty());
}
TEST(test_container, stack_copy_constructor)
{
	stack a = stack(2);
	a.push(lexem("a"));
	stack b = a;
	EXPECT_EQ(a.pop().s_value, b.pop().s_value);
}
TEST(test_container, stack_full)
{
	stack a = stack(2);
	a.push(lexem());
	a.push(lexem());
	EXPECT_EQ(true, a.is_full());
}
TEST(test_container, stack_empty)
{
	stack a = stack(2);
	EXPECT_EQ(true, a.is_empty());
}
TEST(test_container, stack_push)
{
	stack a = stack(2);
	a.push(lexem());
	EXPECT_EQ(false, a.is_empty());
}
TEST(test_container, stack_pop)
{
	stack a = stack(2);
	a.push(lexem("a"));
	EXPECT_EQ("a", a.pop().s_value);
}
TEST(test_container, stack_clear)
{
	stack a = stack(2);
	a.push(lexem());
	a.clear();
	EXPECT_EQ(true, a.is_empty());
}