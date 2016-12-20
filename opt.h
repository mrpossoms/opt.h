//------------------------------------------------------------------------------
//     ___       _         _
//    / _ \ _ __| |_      | |_
//   | (_) | '_ \  _|  _  | ' \
//    \___/| .__/\__| (_) |_||_|
//         |_|
//
//	Set of helpful functions for detecting cli arg flags,
//	key-value pairs and acting on command line arguments.
//

#ifndef _OPT_LIB
#define _OPT_LIB

#include <stdio.h>
#include <string.h>
#include <ctype.h>

typedef struct {
	const char* name;
	const char* description;
	int is_kvp;
	void (*opt)(char* val, int present);
} cli_opt_t;

static int ARGC;
static const char** ARGV;
static int USR_OPT_LIST_C;
static cli_opt_t* USR_OPT_LIST;

// Must be invoked at the beginning of Main()
#define USE_OPT ARGC = argc; ARGV = argv;

// If you are declaring an argument list with handlers
// all declarations must be placed between OPT_LIST_START
// and OPT_LIST_END
#define OPT_LIST_START cli_opt_t OPT_LIST[] = {

#define OPT_LIST_END(HEADER) };\
USR_OPT_LIST_C = sizeof(OPT_LIST) / sizeof(cli_opt_t);\
USR_OPT_LIST = OPT_LIST;\
if(opt_has_flag("-?") || opt_has_flag("--help")){\
	opt_print_usage((HEADER));\
	exit(0);\
}\
for(int i = USR_OPT_LIST_C; i--;){\
	cli_opt_t* o = OPT_LIST + i;\
	if(o->is_kvp){\
		char* value = NULL;\
		int present = opt_has_value(o->name, &value);\
		o->opt(value, present);\
	} else {\
		o->opt(NULL, opt_has_flag(o->name));\
	}\
}\
if(ARGC == 1) opt_print_usage((HEADER));\

static inline void opt_print_usage(const char* header)
{
	if(header) printf("%s", header);

	printf("Usage:\n");
	printf("\t%s [", ARGV[0]);

	for(int i = 0; i < USR_OPT_LIST_C; ++i)
	{
		if(!i) printf("%s ", USR_OPT_LIST[i].name);
		else printf("| %s", USR_OPT_LIST[i].name);
	}

	printf("]\n\n");

	for(int i = 0; i < USR_OPT_LIST_C; ++i)
		printf("\t%s : %s\n", USR_OPT_LIST[i].name, USR_OPT_LIST[i].description);
}
//------------------------------------------------------------------------------
// @param flag - leading substring being searched for in ARGV.
//               flag must be a null terminated string. flag
// @return A non-negative index of the flag string, it is present in ARGV.
//         If flag is not found, -1 is returned instead.
static inline int opt_index_of(const char* flag)
{
	int len = strlen(flag);

	for(int i = ARGC; i--;)
	{
		if(!strncmp(flag, ARGV[i], len))
		{
			return i;
		}
	}

	return -1;
}
//------------------------------------------------------------------------------
// @param flag - leading substring being searched for in ARGV.
//               flag must be a null terminated string.
// @return 1 if flag is found in ARGV, otherwise 0
static inline int opt_has_flag(const char* flag)
{
	return opt_index_of(flag) >= 0;
}
//------------------------------------------------------------------------------
// @param for_key - key for a key-value-pair existing as an argument in ARGV.
//                  The key and value may be present in the same argument so
//                  long as they are joined by a '=' character.
// @param value - if 'for_key' is found and a valid value is also found in
//                ARGV, then value's pointer will be set to said value.
// @return 0 if 'for_key' or its corresponding value are not found. 1 if both
//         are found in ARGV.
static inline int opt_has_value(const char* for_key, const char** value)
{
	int i = opt_index_of(for_key);

	if(i < 0) return 0;

	const char* matched_key = ARGV[i];
	int key_len = strlen(for_key);
	int mk_len  = strlen(matched_key);

	if(mk_len < key_len) return 0;

	// look for a '=' which may follow the matched flag
	// in which case, the value following the flag will be part of
	// the same string
	if(matched_key[key_len] == '=')
	{
		*value = (char*)matched_key + key_len + 1;
		return 1;
	}

	// if we got here, then there was no kv-pair in the argument
	// look for the next argument, is there one?
	if(ARGC > i + 1){
		// then that must be our value
		*value = ARGV[i + 1];
		return 1;
	}

	return 0;
}

#endif
