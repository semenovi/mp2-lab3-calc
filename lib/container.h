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
	int priority();
	bool operator!=(const lexem &v) const;
	lexem operator=(const lexem &s);
};
class functions
{
public:
	std::string name[50];
	std::string body[50];
	std::string input;
	int fts_cnt;
	functions(std::string _input = "") : input(_input), fts_cnt(0) {}
	void add_function(std::string _name, std::string _body);
	functions operator=(const functions &s);
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
	state(std::string _input, lexem _vars[50] = nullptr, int _vars_cnt = 0, functions _fts = functions());
};
class stack
{
	int size, index;
	lexem * memory;
	stack() {};
public:
	stack(int _size) : size(_size), index(-1) { memory = new lexem[size]; }
	stack(const stack & s);
	~stack() { delete[] memory; };
	bool operator==(const stack &s);
	stack operator=(const stack &s);
	bool is_full() { return (index == size - 1); }
	bool is_empty() { return (index == -1); }
	void push(lexem v);
	lexem pop();
	void clear() { index = -1; }
};
class queue
{
	int size, top, end;
	lexem * memory;
public:
	queue(int _size = 20) : size(_size), end(size - 1), top(0) { memory = new lexem[size]; }
	queue(const queue &b);
	bool is_empty()	{ return ((end + 1) % size == top);	}
	bool is_full() { return ((end + 2) % size == top); }
	void push(lexem a);
	lexem pop();
};
#endif
