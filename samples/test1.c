#include "opt.h"
#include <stdio.h>

void has_f_proc(char* val)
{
	printf("Has option -f\n");
}

void has_v_proc(char* val)
{
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
			.has_opt = has_f_proc
		},
		{ 
			.name = "-v",
			.is_kvp = 1,
			.description = "This looks for a v and its value",
			.has_opt = has_v_proc
		},
	OPT_LIST_END

	return 0;
}
