/*-----------------------------------------------------------------------------------*/
/* CORNET Allan */
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "machine.h"
#include "Scierror.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
int C2F(erro)(char *str,long int str_len);
/*-----------------------------------------------------------------------------------*/
 int C2F(erro)(char *str,long int str_len)
{
		int len=strlen(str);
		char *LocalStr=(char*)MALLOC(sizeof(char)*str_len+1);
		if (LocalStr)
		{
			strcpy(LocalStr,str);
			Scierror(9999,LocalStr);
			FREE(LocalStr);
			LocalStr=NULL;
		}
		return 0;
}
/*-----------------------------------------------------------------------------------*/
