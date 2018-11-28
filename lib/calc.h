#ifndef CALC
#define CALC

#include "container.h"
#include <math.h>
class preprocessor
{
	std::string input;
	queue postfix;
	queue prefix;
	functions fts;
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
	void delete_spaces()
	{
		for (int i = 0; i < input.length(); i++)
		{
			if (input[i] == ' ')
			{
				for (int j = i + 1; j < input.length(); j++)
				{
					input[j - 1] = input[j];
				}
				input.resize(input.length() - 1);
			}
		}
	}
	queue create_postfix()
	{
		queue output(input.length() + 1);
		lexem current_double(0.0), current_double_after_dot(0.0), current_string("");
		int mode = 0;
		bool f = false;
		if (!((int)input[0] <= (int)'9' && (int)input[0] >= (int)'0'))
			current_string.s_value.append(&input[0], 1);
		for (int i = 0; i < input.length(); i++)
		{
			// search function name
			if (current_string.s_value != "")
			{
				for (int j = 0; j < fts.fts_cnt; j++)
				{
					if (current_string.s_value == fts.name[j])
					{
						f = true;
					}
				}
			}
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
			else if ((lexem(std::string(1, input[i])).priority() < 0 && lexem(std::string(1, input[i + 1])).priority() > 0) || (lexem(std::string(1, input[i])).priority() < 0 && i + 1 == input.length()) || (lexem(std::string(1, input[i])).priority() < 0 && std::string(1, input[i + 1]) ==")"))
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
				// next line looks useless or not right
				current_string.s_value.append(&input[j + 2], 1);
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
				lexem temp(input.substr(i + 6, j - i - 5));
				temp.d_value = k;
				temp.type = 4;
				output.push(temp);
				current_string.s_value.clear();
				// next line looks useless or not right
				current_string.s_value.append(&input[i + 1], 1);
				i = j + 1;
			}
			// function definition
			else if (input.substr(i, 8) == "function")
			{
				int j = i + 8, k = 0;
				while (input[j + 1] != '{')
				{
					j++;
				}
				k = j + 1;
				std::string name = input.substr(i + 8, j - i - 7);
				int cntu = 1;
				while (cntu != 0)
				{
					if (input[k + 1] == '{')
					{
						cntu++;
					}
					else if (input[k + 1] == '}')
					{
						cntu--;
					}
					k++;
				}
				k--;
				std::string body = input.substr(j + 2, k - j - 1);
				fts.add_function(name, body);
				current_string.s_value.clear();
				current_string.s_value.append(&input[k + 2], 1);
				i = k + 1;
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
			// function call
			else if (f)
			{
				int j = i + current_string.s_value.length();
				while (input[j] != ')')
				{
					j++;
				}
				lexem temp(input.substr(i - 2, j - i + 3));
				temp.type = 5;
				output.push(temp);
				current_string.s_value.clear();
				i = j;
			}
			// part of lexems
			else
			{
				current_string.s_value.append(&input[i+1], 1);
			}
			f = false;
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
			if (current.type == 0 || current.type == 2 || current.type == 3 || current.type == 4 || current.type == 5)
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
	functions get_functions()
	{
		return fts;
	}
	preprocessor(std::string _input, functions _fts = functions())
	{
		input = _input;
		int error = check_brackets();
		if (error) throw ("Brackets error: " + error);
		fts = _fts;
		unary_minus_to_binary();
		delete_spaces();
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
	state get_result_by_custom_in(state st)
	{
		queue a = preprocessor(st.input, st.fts).get_prefix();
		stack temp(st.input.length());
		lexem b, c, d;
		lexem vars[50];
		for (int i = 0; i < st.vars_cnt; i++)
		{
			vars[i] = st.vars[i];
		}
		int vars_cnt = st.vars_cnt;
		bool f = true;
		while (!a.is_empty())
		{
			f = true;
			b = a.pop();
			// integer
			if (b.type == 0)
			{
				temp.push(b);
			}
			// function call
			else if (b.type == 5)
			{
				int j = 0;
				int m = 0;
				while (b.s_value[m] != '(')
				{
					m++;
				}
				for (j = 0; j < st.fts.fts_cnt; j++)
				{
					if (b.s_value.substr(0, m) == st.fts.name[j])
					{
						break;
					}
				}
				int t = 1, u = m + 1, p = 0, h = 0, l = 0;
				lexem vars[50];
				state stp(st.fts.body[j], vars, 0, st.fts);
				while (st.fts.body[j][t - 1] != ';')
				{
					p = u;
					while (!(b.s_value[u] == ',' || b.s_value[u] == ')'))
					{
						u++;
					}
					t = std::atoi(b.s_value.substr(p, u - p).c_str());
					u++;
					stp.vars[stp.vars_cnt] = lexem(t);
					h = l;
					while (!(st.fts.body[j][l] == ',' || st.fts.body[j][l] == ';'))
					{
						l++;
					}
					stp.vars[stp.vars_cnt].s_value = st.fts.body[j].substr(h, l - h);
					l++;
					stp.vars_cnt++;
					t = l;
				}
				stp = get_result_by_custom_in(stp);
				temp.push(stp.last_output);
			}
			// cycle
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
						break;
					}
				}
				for (int i = 0; i < vars_cnt; i++)
				{
					if (vars[i].s_value[0] == r.s_value[0])
					{
						r.d_value = vars[i].d_value;
						break;
					}
				}

				if (b.d_value == 1)
				{
					while (l.d_value < r.d_value)
					{
						std::string u = b.s_value.substr(i - j + 4, b.s_value.length() - 1);
						state stu(u, vars, vars_cnt);
						stu = get_result_by_custom_in(stu);
						for (int i = 0; i < vars_cnt; i++)
						{
							vars[i] = stu.vars[i];
						}
						for (int i = 0; i < vars_cnt; i++)
						{
							if (vars[i].s_value[0] == l.s_value[0])
							{
								l.d_value = vars[i].d_value;
								break;
							}
						}
						for (int i = 0; i < vars_cnt; i++)
						{
							if (vars[i].s_value[0] == r.s_value[0])
							{
								r.d_value = vars[i].d_value;
								break;
							}
						}
					}
				}
				else if (b.d_value == 2)
				{
					while (l.d_value > r.d_value)
					{
						std::string u = b.s_value.substr(i - j + 4, b.s_value.length() - 1);
						state stu(u, vars, vars_cnt);
						stu = get_result_by_custom_in(stu);
						for (int i = 0; i < vars_cnt; i++)
						{
							vars[i] = stu.vars[i];
						}
						for (int i = 0; i < vars_cnt; i++)
						{
							if (vars[i].s_value[0] == l.s_value[0])
							{
								l.d_value = vars[i].d_value;
								break;
							}
						}
						for (int i = 0; i < vars_cnt; i++)
						{
							if (vars[i].s_value[0] == r.s_value[0])
							{
								r.d_value = vars[i].d_value;
								break;
							}
						}
					}
				}
				else if (b.d_value == 3)
				{
					while (l.d_value == r.d_value)
					{
						std::string u = b.s_value.substr(i - j + 4, b.s_value.length() - 1);
						state stu(u, vars, vars_cnt);
						stu = get_result_by_custom_in(stu);
						for (int i = 0; i < vars_cnt; i++)
						{
							vars[i] = stu.vars[i];
						}
						for (int i = 0; i < vars_cnt; i++)
						{
							if (vars[i].s_value[0] == l.s_value[0])
							{
								l.d_value = vars[i].d_value;
								break;
							}
						}
						for (int i = 0; i < vars_cnt; i++)
						{
							if (vars[i].s_value[0] == r.s_value[0])
							{
								r.d_value = vars[i].d_value;
								break;
							}
						}
					}
				}
			}
			else if (b.type == 3)
			{
				std::string u = b.s_value.substr(2, b.s_value.length() - 2);
				state stu(u, vars, vars_cnt, preprocessor(u, st.fts).get_functions());
				double r = get_result_by_custom_in(stu).last_output;
				// 
				// double r = get_result_by_custom_in(b.s_value.substr(2,b.s_value.length() - 2), vars, vars_cnt);
				//
				//temp.push(r);
				for (int i = 0; i < vars_cnt; i++)
				{
					if (vars[i].s_value[0] == b.s_value[0])
					{
						vars[i].d_value = r;
						f = false;
						break;
					}
				}
				if (f)
				{
					vars[vars_cnt] = lexem(b.s_value[0]);
					vars[vars_cnt].d_value = r;
					vars[vars_cnt].s_value = b.s_value[0];
					vars[vars_cnt].type = 2;
					vars_cnt++;
					f = false;
				}
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
		state pu(st.input, vars, vars_cnt, preprocessor(input, st.fts).get_functions());
		if (!temp.is_empty())
		{
			pu.last_output = temp.pop().d_value;
		}
		return pu;
	}
public:
	std::string input;
	calc(std::string _input) : input(_input) {}
	double get_result()
	{
		return get_result_by_custom_in(input).last_output;
	}
};
#endif
