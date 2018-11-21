#include "container.h"
#include "calc.h"
#include <iostream>
int main()
{
	calc a("while{i<h;34}");
	double b = a.get_result();
	std::cout << b << std::endl;
	return 0;
}
