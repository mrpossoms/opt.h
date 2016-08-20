#include "opt.h"
#include <stdio.h>

void f_proc(char* val, int present)
{
	if(present)
		printf("Has option -f\n");
}

void v_proc(char* val, int present)
{
	if(present)
		printf("Has value '%s'\n", val);
}

int main(int argc, char* argv[])
{
	USE_OPT
	OPT_LIST_START
		{ 
			.name = "-f",
			.is_kvp = 0,
			.description = "This looks for an f",
			.opt = f_proc
		},
		{ 
			.name = "-v",
			.is_kvp = 1,
			.description = "This looks for a v and its value",
			.opt = v_proc
		},
	OPT_LIST_END("Sample1")

	return 0;
}
