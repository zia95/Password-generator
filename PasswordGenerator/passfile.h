#pragma once

#include "../../utl/utl/utl_file.h"




#define FILE_PASSWORD_LIST "./pswdlist.txt"



bool save_pswd(const char* pswd)
{
	if (!pswd)
		return false;
	FILE* f = fopen(FILE_PASSWORD_LIST, "a");
	if (f)
	{
		if (file_get_size(f) == 0)
			fprintf(f, "%s", pswd);
		else
			fprintf(f, "\n%s", pswd);
		return (fclose(f) == 0);
	}
	return false;
}
bool list_all_pswd()
{
	FILE* f = fopen(FILE_PASSWORD_LIST, "r");
	if (f)
	{
		int ln = 1;
		int ch = 0;
		printf("\n%3d. ", ln);
		while ((ch = fgetc(f)) != EOF)
		{
			printf("%c", ch);
			if (ch == '\n')
			{
				ln++;
				printf("%3d. ", ln);
			}
		}
		printf("\n");
		if (fclose(f) == 0)
			return true;
	}
	return false;
}

bool rmv_ln_no(int nLnNo)
{
	long file_len;
	char* buffer;
	FILE* f = fopen(FILE_PASSWORD_LIST, "r+");
	if (f)
	{
		file_len = file_get_size(f);
		if (file_len > 0)
		{
			buffer = (char*)malloc(file_len);
			if (buffer)
			{
				memset(buffer, 0, file_len);



				// read line by line except remove line
				int cur_idx = 0;
				int ln = 1;
				int ch = 0;
				while ((ch = fgetc(f)) != EOF)
				{
					if (ln != nLnNo)
					{
						buffer[cur_idx] = ((char)ch);
						cur_idx++;
					}
					
					if (ch == '\n')
						ln++;
				}


				if (ln == nLnNo)// if last line was the one to be removed then erase '\n' from 2nd-last one
				{
					if (buffer[--cur_idx] == '\n')
						buffer[cur_idx] = L'\0';
				}

				if (fclose(f) == 0)
				{
					return file_writetexta(FILE_PASSWORD_LIST, buffer, false);
				}

				
				/*
				if (freopen(FILE_PASSWORD_LIST, "w", f) == f)
				{
					if (fwrite(buffer, file_len, 1, f) == 1)
					{
						if (fclose(f) == 0)
							return EXIT_SUCCESS;
					}
				}
				*/
			}

		}




	}
	return false;
}