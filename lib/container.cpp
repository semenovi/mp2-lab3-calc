#include "container.h"
int lexem::priority()
{
	if (s_value == "+")
		return 1;
	else if (s_value == "-")
		return 1;
	else if (s_value == "*")
		return 2;
	else if (s_value == "/")
		return 2;
	else if (s_value == "(")
		return 0;
	else if (s_value == ")")
		return 0;
	else if (s_value == "sin")
		return 3;
	return -1;
}
bool lexem::operator!=(const lexem &v) const
{
	if (type == 0)
		return d_value != v.d_value;
	else
		return s_value != v.s_value;
}
lexem lexem::operator=(const lexem &s)
{
	type = s.type;
	d_value = s.d_value;
	s_value = s.s_value;
	return *this;
}
void functions::add_function(std::string _name, std::string _body)
{
	name[fts_cnt] = _name;
	body[fts_cnt] = _body;
	fts_cnt++;
}
functions functions::operator=(const functions &s)
{
	input = s.input;
	fts_cnt = s.fts_cnt;
	for (int i = 0; i <= s.fts_cnt; i++)
	{
		name[i] = s.name[i];
		body[i] = s.body[i];
	}
	return *this;
}
state::state(std::string _input, lexem _vars[50], int _vars_cnt, functions _fts)
{
	input = _input;
	if (_vars != nullptr)
	{
		for (int i = 0; i < _vars_cnt; i++)
		{
			vars[i] = _vars[i];
		}
	}
	vars_cnt = _vars_cnt;
	fts = _fts;
	last_output = 0;
}
stack::stack(const stack & s) : size(s.size), index(s.index)
{
	memory = new lexem[size];
	for (int i = 0; i <= s.index; i++)
		memory[i] = s.memory[i];
}
bool stack::operator==(const stack &s)
{
	if (&s == this)
		return 1;
	if ((size == s.size) && (index == s.index))
	{
		for (int i = 0; i <= index; i++)
			if (memory[i] != s.memory[i])
				return 0;
		return 1;
	}
	return 0;
}
stack stack::operator=(const stack &s)
{
	size = s.size;
	index = s.index;
	for (int i = 0; i <= s.index; i++)
		memory[i] = s.memory[i];
	return *this;
}
void stack::push(lexem v)
{
	if (is_full()) throw(-1);
	index++;
	memory[index] = v;
}
lexem stack::pop()
{
	if (is_empty()) throw(-2);
	index--;
	return memory[index + 1];
}
queue::queue(const queue &b) : size(b.size), end(b.end), top(b.top)
{
	memory = new lexem[size];
	if (is_empty()) return;
	int i = top;
	while (i != end + 1)
	{
		memory[i] = b.memory[i];
		i = (i + 1) % size;
	}
}
void queue::push(lexem a)
{
	if (is_full()) throw(-1);
	end = (end + 1) % size;
	memory[end] = a;
}
lexem queue::pop()
{
	if (is_empty()) throw(-1);
	lexem k = memory[top];
	top = (top + 1) % size;
	return k;
}
