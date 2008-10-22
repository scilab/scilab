/*------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* Authors : Cong Wu                                                      */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "strsplit.h"
/*------------------------------------------------------------------------*/
void strsplit(char **Input_StringMatrix,char **Output_StringMatrix,int *Row_Two,int *Col_Two,int *Col_Pointer,int *Input_IntMatrix)
{
	int x = 0;
	int i=0;
	int l;
	strncpy(Output_StringMatrix[0],Input_StringMatrix[0],Input_IntMatrix[0]);
    for (x = 1; x <= (int)strlen(Input_IntMatrix)+1 ; x++)
	{
		for(i=Input_IntMatrix[x-1];i<Input_IntMatrix[x];i++)
		{
             Output_StringMatrix[x][i-Input_IntMatrix[x-1]]=Input_StringMatrix[0][i];
		}
		
        Output_StringMatrix[x][Input_IntMatrix[x]-Input_IntMatrix[x-1]]='\0';
	}
	l=(*Row_Two)*(*Col_Two);
    if (Input_IntMatrix[l-1]<(int)strlen(Input_StringMatrix[0])) 
	{
		for (i=Input_IntMatrix[l-1];i<=(int)strlen(Input_StringMatrix[0]);i++)
		{
			Output_StringMatrix[l][i-Input_IntMatrix[l-1]]=Input_StringMatrix[0][i];
		}
	}


}
/*------------------------------------------------------------------------*/
