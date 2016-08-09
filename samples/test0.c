#include "opt.h"
#include <stdio.h>

int main(int argc, char* argv[])
{
	USE_OPT

	if(opt_has_flag("-f")){
		printf("Has option -f\n");
	}

	char* value = NULL;
	if(opt_has_value("-v", &value)){
		printf("Has value '%s'\n", value);
	}	

	return 0;
}
