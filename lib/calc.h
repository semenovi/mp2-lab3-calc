#ifndef CALC
#define CALC

#include "container.h"
#include <math.h>
class preprocessor
{
	std::string input;
	queue postfix;
	queue prefix;
	// returns 0 if all right
	int check_brackets()
	{
		int c = 0, last = 0;
		for (int i = 0; i < input.length(); i++)
		{
			if (input[i] == '(')
			{
				c++;
				last = i;
			}
			else if (input[i] == ')')
			{
				c--;
			}
			if (c < 0) return i;
		}
		if (c == 0) return 0;
		return last;
	}
	void unary_minus_to_binary()
	{
		if (input[0] == '-')
		{
			input.resize(input.length() + 1);
			input[input.length()] = input[input.length() - 1];
			for (int j = input.length() - 1; j >= 0; j--)
			{
				input[j + 1] = input[j];
			}
			input[0] = '0';

		}
		for (int i = 0; i < input.length(); i++)
		{
			if (input[i] == '(' && input[i + 1] == '-')
			{
				input.resize(input.length() + 1);
				input[input.length()] = input[input.length() - 1];
				for (int j = input.length() - 2; j >= i; j--)
				{
					input[j + 1] = input[j];
				}
				input[i + 1] = '0';
			}
		}
	}
	queue create_postfix()
	{
		queue output(input.length() + 1);
		lexem current_double(0.0), current_double_after_dot(0.0), current_string("");
		int mode = 0;
		if (!((int)input[0] <= (int)'9' && (int)input[0] >= (int)'0'))
			current_string.s_value.append(&input[0], 1);
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
					if (mode == 0)
					{
						current_double.d_value = current_double.d_value * 10.0;
						current_double.d_value += ((int)input[i] - (int)'0');
						if (input[i + 1] != '.')
							output.push(current_double);
					}
					else
					{
						current_double_after_dot.d_value = current_double_after_dot.d_value * 10.0;
						current_double_after_dot.d_value += ((int)input[i] - (int)'0');
						while (current_double_after_dot.d_value > 1)
							current_double_after_dot.d_value = current_double_after_dot.d_value / 10;
						current_double.d_value += current_double_after_dot.d_value;
						output.push(current_double);
					}
					if (input[i + 1] != '.')
					{
						current_double.d_value = 0.0;
						mode = 0;
						current_string.s_value.append(&input[i + 1], 1);
					}
				}
				else
				{
					if (mode == 0)
					{
						current_double.d_value = current_double.d_value * 10.0;
						current_double.d_value += ((int)input[i] - (int)'0');
					}
					else
					{
						current_double_after_dot.d_value = current_double_after_dot.d_value * 10.0;
						current_double_after_dot.d_value += ((int)input[i] - (int)'0');
					}
				}
			}
			// dot
			else if (input[i] == '.')
			{
				mode = 1;
			}
			// variable
			else if ((lexem(std::string(1, input[i])).priority() < 0 && lexem(std::string(1, input[i + 1])).priority() > 0) || (lexem(std::string(1, input[i])).priority() < 0 && i + 1 == input.length() ))
			{
				lexem temp(std::string(1, input[i]));
				temp.type = 2;
				output.push(temp);
				current_string.s_value.clear();
				current_string.s_value.append(&input[i + 1], 1);
			}
			// initialization
			else if (lexem(std::string(1, input[i])).priority() < 0 && std::string(1, input[i + 1]) == "=")
			{
				int j = i + 2;
				while (input[j + 1] != ';')
				{
					j++;
				}
				lexem temp(input.substr(i, j - i + 1));
				temp.type = 3;
				output.push(temp);
				current_string.s_value.clear();
				current_string.s_value.append(&input[i + 1], 1);
				i = j + 1;
			}
			// cycle
			else if (input.substr(i, 6) == "while{")
			{
				int j = i + 6, k = 0;
				while (input[j + 1] != '}')
				{
					if (k == 0)
					{
						if (input[j] == '<')
							k = 1;
						else if (input[j] == '>')
							k = 2;
						else if (input[j] == '=')
							k = 3;
					}
					j++;
				}
				lexem temp(input.substr(i + 6, j - i - 6));
				temp.d_value = k;
				temp.type = 4;
				output.push(temp);
				current_string.s_value.clear();
				current_string.s_value.append(&input[i + 1], 1);
				i = j + 1;
			}
			// lexems
			else if (current_string.priority() > -1)
			{
				output.push(current_string);
				current_string.s_value.clear();
				// next char isnt a digit
				if (!((int)input[i + 1] <= (int)'9' && (int)input[i + 1] >= (int)'0'))
				{
					current_string.s_value.append(&input[i + 1], 1);
				}
				else if (lexem(input[i + 1]).priority() > 0)
				{
					throw(-4);
				}
			}
			// part of lexems
			else
			{
				current_string.s_value.append(&input[i+1], 1);
			}
		}
		return output;
	}
	queue create_prefix()
	{
		queue input_queue = postfix, output(input.length() + 1);
		stack operators(input.length());
		lexem current(0.0);
		int max_priority = -2;
		while (!input_queue.is_empty())
		{
			current = input_queue.pop();
			// integers or variables
			if (current.type == 0 || current.type == 2 || current.type == 3 || current.type == 4)
			{
				output.push(current);
			}
			// lexems
			else
			{
				// operators
				if (current.priority() > 0)
				{
					// stack isnt empty
					if (!operators.is_empty())
					{
						lexem a = operators.pop();
						operators.push(a);
						while (a.priority() >= current.priority())
						{
							a = operators.pop();
							output.push(a);
							if (operators.is_empty()) break;
							a = operators.pop();
							operators.push(a);
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
						lexem a = operators.pop();
						while (a.s_value != "(")
						{
							output.push(a);
							a = operators.pop();
						}
						if (!operators.is_empty())
						{
							a = operators.pop();
							operators.push(a);
						}
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
	preprocessor(std::string _input)
	{
		input = _input;
		int error = check_brackets();
		if (error) throw ("Brackets error: " + error);
		unary_minus_to_binary();
		postfix = create_postfix();
		prefix = create_prefix();
	}
	queue get_postfix()
	{
		return postfix;
	}
	queue get_prefix()
	{
		return prefix;
	}
};

class calc
{
	double get_result_by_custom_in(std::string input, lexem _vars[50] = nullptr, int _vars_cnt = 0)
	{
		queue a = preprocessor(input).get_prefix();
		stack temp(input.length());
		lexem b, c, d;
		lexem vars[50];
		if (_vars != nullptr)
		{
			for (int i = 0; i < 50; i++)
			{
				vars[i] = _vars[i];
			}
		}
		int vars_cnt = _vars_cnt;
		bool f = true;
		while (!a.is_empty())
		{
			f = true;
			b = a.pop();
			if (b.type == 0)
			{
				temp.push(b);
			}
			else if (b.type == 4)
			{
				lexem l, r;
				int i = 0, j = 0;
				while (b.s_value[i + 1] != ';')
				{
					if (b.s_value[i + 1] == '<' || b.s_value[i + 1] == '>' || b.s_value[i + 1] == '=')
					{
						l = lexem(b.s_value.substr(0, i + 1));
						j = i + 2;
					}
					i++;
				}
				r = lexem(b.s_value.substr(j, i - j + 1));
				for (int i = 0; i < vars_cnt; i++)
				{
					if (vars[i].s_value[0] == l.s_value[0])
					{
						l.d_value = vars[i].d_value;
						continue;
					}
					if (vars[i].s_value[0] == r.s_value[0])
					{
						r.d_value = vars[i].d_value;
						continue;
					}
				}
				if (b.d_value == 1)
				{
					while (l.d_value < r.d_value)
					{
						
					}
				}
				else if (b.d_value == 2)
				{
					while (l.d_value > r.d_value)
					{
						
					}
				}
				else if (b.d_value == 3)
				{
					while (l.d_value == r.d_value)
					{

					}
				}
			}
			else if (b.type == 3)
			{
				double r = get_result_by_custom_in(b.s_value.substr(2,b.s_value.length() - 2), vars, vars_cnt);
				temp.push(r);
				vars[vars_cnt] = lexem(b.s_value[0]);
				vars[vars_cnt].d_value = r;
				vars[vars_cnt].s_value = b.s_value[0];
				vars[vars_cnt].type = 2;
				vars_cnt++;
			}
			else if (b.type == 2)
			{
				for (int i = 0; i < vars_cnt; i++)
				{
					if (vars[i].s_value[0] == b.s_value[0])
					{
						temp.push(vars[i].d_value);
						f = false;
						break;
					}
				}
				if (f)
				{
					std::cout << "Enter variable " << b.s_value[0] << ":" << std::endl << "> ";
					std::cin >> vars[vars_cnt].d_value;
					temp.push(vars[vars_cnt].d_value);
					vars[vars_cnt].s_value[0] = b.s_value[0];
					vars_cnt++;
					f = false;
				}
			}
			else
			{
				if (b.s_value == "+")
				{
					c = temp.pop();
					d = temp.pop();
					temp.push(d.d_value + c.d_value);
				}
				else if (b.s_value == "-")
				{
					c = temp.pop();
					d = temp.pop();
					temp.push(d.d_value - c.d_value);
				}
				else if (b.s_value == "*")
				{
					c = temp.pop();
					d = temp.pop();
					temp.push(d.d_value * c.d_value);
				}
				else if (b.s_value == "/")
				{
					c = temp.pop();
					d = temp.pop();
					temp.push(d.d_value / c.d_value);
				}
				else if (b.s_value == "sin")
				{
					c = temp.pop();
					temp.push(sin(c.d_value));
				}
			}
		}
		return temp.pop().d_value;
	}
public:
	std::string input;
	calc(std::string _input) : input(_input) {}
	double get_result()
	{
		return get_result_by_custom_in(input);
	}
};
#endif
