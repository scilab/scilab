/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "CreateEmptystr.h"
/*-----------------------------------------------------------------------------------*/
char ** CreateEmptystr(int m1,int n1)
{
	char **OutputStrings = (char**)MALLOC(sizeof(char*)*(m1*n1+1)); 

	if (OutputStrings)
	{
		int i = 0;
		for (i = 0;i < m1*n1; i++)  /*m1 is the number of row ; n1 is the number of col*/
		{
			OutputStrings[i] = (char*)MALLOC(sizeof(char)*(strlen(EMPTY_STRING)+1));
			if (OutputStrings[i])
			{
				strcpy(OutputStrings[i],EMPTY_STRING);
			}
			else
			{
				for (i=0;i<m1*n1;i++)
				{
					if (OutputStrings[i]) { FREE(OutputStrings[i]); OutputStrings[i] = NULL;}
				}
				if (OutputStrings) {FREE(OutputStrings); OutputStrings = NULL; }

				Scierror(999, "MALLOC error of output matrix");
				break;
			}
		}
	}
	return OutputStrings;
}
/*-----------------------------------------------------------------------------------*/
