#include "kernel.h"
#include <iostream.h>
#include <stdlib.h>

int userMain(int argc, char* argv[]){ return 0;}


int main(int argc, char* argv[])
{
	Kernel::initAll();
	int ret = userMain(argc, argv);
	Kernel::unInitAll();
	return ret;
}


