/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "returnProperty.h"
#include "machine.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "CreateEmptystr.h"
/*-----------------------------------------------------------------------------------*/
char ** CreateEmptystr(int m1,int n1)
{
	int i = 0;
	char **OutputStrings=NULL;
	OutputStrings = (char**)MALLOC(sizeof(char*)*(m1*n1+1)); 
	if (OutputStrings)
	{
		for (i = 0;i < m1*n1; i++)  /*m1 is the number of row ; n1 is the number of col*/
		{
			OutputStrings[i]=(char*)MALLOC(sizeof(char)*(strlen(EMPTY_STRING)+1));
			if (OutputStrings[i])
			{
				strcpy(OutputStrings[i],EMPTY_STRING);
			}
			else
			{
				Scierror(999, "MALLOC error of output matrix");
				for (i=0;i<m1*n1;i++)
					{
						if (OutputStrings[i]) { FREE(OutputStrings[i]); OutputStrings[i]=NULL;}
					}
				if (OutputStrings) {FREE(OutputStrings); OutputStrings=NULL; }
			
				break;
			}

		}
	}
	return OutputStrings;
}
/*-----------------------------------------------------------------------------------*/
