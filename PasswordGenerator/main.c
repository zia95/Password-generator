#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include "passfile.h"


//#include "utl_asciigen.h"
#include "../../utl/utl/utl_asciigen.h"
#include "../../utl/utl/utl_str.h"



#define ERROR_PARAM_COUNT 1
#define ERROR_PARAM_INVALID 2


#define ERROR_GEN_FAILED 3
#define ERROR_GEN_SAVE_PASS_FAILED 4




#define ERROR_RMV_LIST_FAILED 5
#define ERROR_DISPLAY_LIST_FAILED 6



byte g_gencharflags;

long g_nPassLen;

void print_help()
{
	printf("To generate password-> <password_len> <options>\n\t<options>=l:lower, u:upper, n:number, s:special\n");
	printf("To list saved password-> list\n");
	printf("To remove saved password-> list remove <LineNo>\n");
}




bool try_parse_int(const char* str, long* result)
{
	char* pEnd;
	*result = strtol(str, &pEnd, 10);
	return !(*pEnd);
}
bool try_parse_opts(const char* _str)
{
	g_gencharflags = GC_NONE;
	if (_str)
	{
		for (const char* c = _str; *c != '\0'; c++)
		{
			int l = tolower(*c);
			if (l == 'l')
				g_gencharflags |= GC_LOWER;
			else if (l == 'u')
				g_gencharflags |= GC_UPPER;
			else if (l == 'n')
				g_gencharflags |= GC_NUMBER;
			else if (l == 's')
				g_gencharflags |= GC_SYMBOL;
		}
	}
	return g_gencharflags != GC_NONE;
}
int gen_and_print(long len)
{
	size_t siz = (len + 1);// +1 for '\0'
	char* buffer = (char*)malloc(siz);
	if (buffer == NULL)
		return false;
	memset(buffer, 0, siz);
	asciigen_seed();

	

	bool bSucess = asciigen_gen_char_seq(buffer, len, g_gencharflags);
	if (bSucess)
	{
		printf("Generated Password: '%s'\n", buffer);
		printf("Do you want to save the password? [y/N]");
		int c = getchar();
		if (c == 'Y' || c == 'y')
		{
			printf("saving password....");
			printf("(%s)\n", FILE_PASSWORD_LIST);
			if (save_pswd(buffer))
			{
				printf("pasword saved...\n");
				return EXIT_SUCCESS;
			}
			else
			{
				printf("pasword failed to save...\n");
				return ERROR_GEN_SAVE_PASS_FAILED;
			}
		}
		else
		{
			return EXIT_SUCCESS;
		}

	}
	else
	{
		printf("failed to generate password.\n");
	}
	free(buffer);
	return bSucess;
}

int main(int argc, char* argv[])
{
	if (argc >= 4)
	{
		long rmv_lineNo;
		if (str_equi(argv[1], "list") && str_equi(argv[2], "remove") && try_parse_int(argv[3], &rmv_lineNo))
		{
			if (rmv_ln_no(rmv_lineNo))
			{
				printf("removed sucessfully...\n");
				return EXIT_SUCCESS;
			}
			else
			{
				printf("failed to remove password...\n");
				return ERROR_RMV_LIST_FAILED;
			}
			
		}
		return ERROR_PARAM_INVALID;
	}
	else if (argc >= 3)
	{

		if (try_parse_int(argv[1], &g_nPassLen))
		{
			if (try_parse_opts(argv[2]))
			{
				return gen_and_print(g_nPassLen);
			}
		}
		return ERROR_PARAM_INVALID;
	}
	else if (argc >= 2)
	{
		if (str_equi(argv[1], "list"))
		{
			if (list_all_pswd())
			{
				return EXIT_SUCCESS;
			}
			else
			{
				//printf("removed sucessfully...\n");
				return ERROR_DISPLAY_LIST_FAILED;
			}
		}
		return ERROR_PARAM_INVALID;
	}


	print_help();
	return ERROR_PARAM_COUNT;
}