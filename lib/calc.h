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
	int check_brackets();
	void unary_minus_to_binary();
	void delete_spaces();
	queue create_postfix();
	queue create_prefix();
public:
	functions get_functions() { return fts; }
	preprocessor(std::string _input, functions _fts = functions());
	queue get_postfix()	{ return postfix; }
	queue get_prefix()	{ return prefix; }
};

class calc
{
	state get_result_by_custom_in(state st);
public:
	std::string input;
	calc(std::string _input) : input(_input) {}
	double get_result()	{ return get_result_by_custom_in(input).last_output; }
};
#endif
