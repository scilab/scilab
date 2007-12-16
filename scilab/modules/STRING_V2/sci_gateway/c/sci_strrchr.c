/*----------------------------------------------------------------------------*/
/* Copyright INRIA 2007 */
/* @Author : Allan CORNET */
/*----------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "stringsstrrchr.h"
/*----------------------------------------------------------------------------*/
int C2F(sci_strrchr) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(2,2);
	CheckLhs(0,1);

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
	{
		int m1 = 0, n1 = 0;
		char **Input_StringMatrix_1 = NULL;
		int m1n1 = 0;

		int m2 = 0, n2 = 0;
		char **Input_StringMatrix_2 = NULL;
		int m2n2 = 0;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Input_StringMatrix_1);
		m1n1 = m1 * n1 ;

		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Input_StringMatrix_2);
		m2n2 = m2 * n2 ;

		if ( (m1n1 == m2n2) || (m2n2 == 1) )
		{
			char **Output_Strings = NULL;
			int i = 0;

			for(i = 0;i < m2n2; i++)
			{
				if (strlen(Input_StringMatrix_2[i]) != 1)
				{
					freeArrayOfString(Input_StringMatrix_1,m1n1);
					freeArrayOfString(Input_StringMatrix_2,m2n2);
					Scierror(999,"%s : 2nd parameter must be a character.\n",fname);
					return 0;
				}
			}

			Output_Strings = strings_strrchr(Input_StringMatrix_1,m1n1,Input_StringMatrix_2,m2n2,TRUE);
			if (Output_Strings)
			{
				CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,Output_Strings);
				LhsVar(1) = Rhs+1 ;
				C2F(putlhsvar)();

				freeArrayOfString(Input_StringMatrix_1,m1n1);
				freeArrayOfString(Input_StringMatrix_2,m2n2);
				freeArrayOfString(Output_Strings,m1n1);
			}
			else
			{
				freeArrayOfString(Input_StringMatrix_1,m1n1);
				freeArrayOfString(Input_StringMatrix_2,m2n2);
				Scierror(999,"%s: Out of memory\r\n",fname);
			}
		}
		else
		{
			freeArrayOfString(Input_StringMatrix_1,m1n1);
			freeArrayOfString(Input_StringMatrix_2,m2n2);
			Scierror(999,_("%s : Invalid size second parameter.\n"),fname);
			return 0;
		}
	}
	else
	{
		Scierror(999,_("%s : Invalid input parameter(s).\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
