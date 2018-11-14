#ifndef CONTAINER
#define CONTAINER
#include <iostream>
class unit
{
public:
	int type; //0 - double, 1 - string
	unit() : d_value(0), s_value(""), type(0) {}
	unit(double v) : d_value(v), s_value(""), type(0) {}
	unit(std::string v) : d_value(0), s_value(v), type(1) {}
	double d_value;
	std::string s_value;
	bool operator!=(const unit &v) const
	{
		if (type == 0)
			return d_value != v.d_value;
		else
			return s_value != v.s_value;
	}
	unit operator=(const unit &s)
	{
		type = s.type;
		d_value = s.d_value;
		s_value = s.s_value;
		return *this;
	}
};
class stack
{
	int size, index;
	unit * memory;
	stack() {};
public:
	stack(int _size) : size(_size), index(-1)
	{
		memory = new unit[size];
	}
	stack(const stack & s) : size(s.size), index(s.index)
	{
		memory = new unit[size];
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
	void push(unit v)
	{
		if (is_full()) throw(-1);
		index++;
		memory[index] = v;
	}
	unit pop()
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
	unit * memory;
public:
	queue(int _size = 20) : size(_size), end(size - 1), top(0)
	{
		memory = new unit[size];
	}
	queue(const queue &b) : size(b.size), end(b.end), top(b.top)
	{
		memory = new unit[size];
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
	void push(unit a)
	{
		if (is_full()) throw(-1);
		end = (end + 1) % size;
		memory[end] = a;
	}
	unit pop()
	{
		if (is_empty()) throw(-1);
		unit k = memory[top];
		top = (top + 1) % size;
		return k;
	}
};
#endif
