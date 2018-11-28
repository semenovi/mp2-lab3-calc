#include "container.h"
#include "calc.h"
#include <iostream>
int main()
{
	//calc a("a = 0; b = 2; t = 2; while{a < b; a = a + 1; t = t * t;} t");
	calc a("function lol{a; t = 2; while{a<b; a=a+1; t=2*t;}t} a=lol(90); a");
	double b = a.get_result();
	std::cout << b << std::endl;
	return 0;
}
