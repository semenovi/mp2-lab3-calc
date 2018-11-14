#include "container.h"
#include "calc.h"
#include <iostream>
int main()
{
	calc a("-123+(45-8*65)*3");
	a.print_reversed();
	return 0;
}
