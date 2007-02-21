/*-----------------------------------------------------------------------------------*/
/* CORNET Allan */
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "machine.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "libinter.h"
/*-----------------------------------------------------------------------------------*/
int C2F(erro)(char *str,unsigned int str_len)
{
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
