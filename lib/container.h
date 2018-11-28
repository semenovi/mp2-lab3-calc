#ifndef CONTAINER
#define CONTAINER
#include <iostream>
class lexem
{
public:
	int type; //0 - double, 1 - string, 2 - variable, 3 - init, 4 - cycle, 5 - function call
	lexem() : d_value(0), s_value(""), type(0) {}
	lexem(double v) : d_value(v), s_value(""), type(0) {}
	lexem(std::string v) : d_value(0), s_value(v), type(1) {}
	double d_value;
	std::string s_value;
	int priority()
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
	bool operator!=(const lexem &v) const
	{
		if (type == 0)
			return d_value != v.d_value;
		else
			return s_value != v.s_value;
	}
	lexem operator=(const lexem &s)
	{
		type = s.type;
		d_value = s.d_value;
		s_value = s.s_value;
		return *this;
	}
};
class functions
{
public:
	std::string name[50];
	std::string body[50];
	std::string input;
	int fts_cnt;
	functions(std::string _input = "") : input(_input), fts_cnt(0) {}
	void add_function(std::string _name, std::string _body)
	{
		name[fts_cnt] = _name;
		body[fts_cnt] = _body;
		fts_cnt++;
	}
	functions operator=(const functions &s)
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
};
class state
{
	//std::string input, lexem _vars[50] = nullptr, int _vars_cnt = 0
public:
	std::string input;
	lexem vars[50];
	int vars_cnt;
	double last_output;
	functions fts;
	state(std::string _input, lexem _vars[50] = nullptr, int _vars_cnt = 0, functions _fts = functions())
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
};
class stack
{
	int size, index;
	lexem * memory;
	stack() {};
public:
	stack(int _size) : size(_size), index(-1)
	{
		memory = new lexem[size];
	}
	stack(const stack & s) : size(s.size), index(s.index)
	{
		memory = new lexem[size];
		for (int i = 0; i <= s.index; i++)
			memory[i] = s.memory[i];
	}
	~stack() { delete[] memory; };
	bool operator==(const stack &s)
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
	stack operator=(const stack &s)
	{
		size = s.size;
		index = s.index;
		for (int i = 0; i <= s.index; i++)
			memory[i] = s.memory[i];
		return *this;
	}
	bool is_full() { return (index == size - 1); }
	bool is_empty() { return (index == -1); }
	void push(lexem v)
	{
		if (is_full()) throw(-1);
		index++;
		memory[index] = v;
	}
	lexem pop()
	{
		if (is_empty()) throw(-2);
		index--;
		return memory[index + 1];
	}
	void clear()
	{
		index = -1;
	}
};
class queue
{
	int size, top, end;
	lexem * memory;
public:
	queue(int _size = 20) : size(_size), end(size - 1), top(0)
	{
		memory = new lexem[size];
	}
	queue(const queue &b) : size(b.size), end(b.end), top(b.top)
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
	bool is_empty()
	{
		return ((end + 1) % size == top);
	}
	bool is_full()
	{
		return ((end + 2) % size == top);
	}
	void push(lexem a)
	{
		if (is_full()) throw(-1);
		end = (end + 1) % size;
		memory[end] = a;
	}
	lexem pop()
	{
		if (is_empty()) throw(-1);
		lexem k = memory[top];
		top = (top + 1) % size;
		return k;
	}
};
#endif
