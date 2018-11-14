#ifndef CALC
#define CALC

#include "container.h"
class calc
{
private:
	int priority(unit v)
	{
		if (v.s_value == "+")
			return 1;
		else if (v.s_value == "-")
			return 1;
		else if (v.s_value == "*")
			return 2;
		else if (v.s_value == "/")
			return 2;
		else if (v.s_value == "(")
			return 0;
		else if (v.s_value == ")")
			return 0;
		return -1;
	}
	bool check_brackets()
	{
		int c = 0;
		for (int i = 0; i < input.length(); i++)
		{
			if (input[i] == '(')
				c++;
			else if (input[i] == ')')
				c--;
			if (c < 0) return false;
		}
		if (c == 0) return true;
		return false;
	}
	queue to_lexems()
	{
		if (!check_brackets()) throw (-3);
		{
			unit a(input[0]);
			std::cout << priority(a);
			if (priority(a) > 0) throw (-5);
		}
		queue output(input.length());
		unit current_double(0.0), current_string("");
		for (int i = 0; i < input.length(); i++)
		{
			// integers
			if ((int)input[i] <= (int)'9' && (int)input[i] >= (int)'0')
			{
				// next char isnt a digit
				if (!((int)input[i + 1] <= (int)'9' && (int)input[i + 1] >= (int)'0'))
				{
					if (&input[i + 1] == ")")
					{
						output.push(current_string);
						current_string.s_value.clear();
						current_string.s_value.append(&input[i], 1);
					}
					current_double.d_value = current_double.d_value * 10.0;
					current_double.d_value += ((int)input[i] - (int)'0');
					output.push(current_double);
					current_double.d_value = 0.0;
					current_string.s_value.append(&input[i + 1], 1);
				}
				else
				{
					current_double.d_value = current_double.d_value * 10.0;
					current_double.d_value += ((int)input[i] - (int)'0');
				}
			}
			// lexems
			else if (priority(current_string) > -1)
			{
				output.push(current_string);
				current_string.s_value.clear();
				// next char isnt a digit
				if (!((int)input[i + 1] <= (int)'9' && (int)input[i + 1] >= (int)'0'))
				{
					current_string.s_value.append(&input[i + 1], 1);
				}
				else if (priority(input[i + 1]) > 0)
				{
					throw(-4);
				}
			}
			// part of lexems
			else
			{
				current_string.s_value.append(&input[i], 1);
			}
		}
		return output;
	}
	queue to_poland()
	{
		queue input_queue = to_lexems(), output(input.length());
		stack operators(input.length());
		unit current(0.0);

		while (!input_queue.is_empty())
		{
			current = input_queue.pop();
			// integers
			if (current.type == 0)
			{
				output.push(current);
			}
			// lexems
			else
			{
				// operators
				if (priority(current) > 0)
				{
					// stack isnt empty
					if (!operators.is_empty())
					{
						unit a = operators.pop();
						operators.push(a);
						while (priority(a) >= priority(current))
						{
							output.push(current);
							current = operators.pop();
						}
						operators.push(current);
					}
					else
					{
						operators.push(current);
					}
				}
				// brackets
				else
				{
					if (current.s_value == "(")
					{
						operators.push(current);
					}
					else
					{
						unit a = operators.pop();
						while (a.s_value != "(")
						{
							output.push(a);
							a = operators.pop();
						}
						a = operators.pop();
						operators.push(a);
					}
				}
			}
		}
		while (!operators.is_empty())
		{
			current = operators.pop();
			output.push(current);
		}
		return output;
	}
public:
	std::string input;
	calc(std::string _input) : input(_input) {};
	void print_reversed()
	{
		queue a = to_poland();
		unit temp(0.0);
		while (!a.is_empty())
		{
			temp = a.pop();
			if (temp.type == 0)
				std::cout << temp.d_value << " ";
			else
			{
				for (int i = 0; i < temp.s_value.length(); i++)
					std::cout << temp.s_value[i];
				std::cout << " ";
			}
			std::cout << std::endl;
		}
	}
};
#endif
