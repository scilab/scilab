/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006/2007 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Pierre MARECHAL
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "gw_core.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "md5.h"
#include "localization.h"
#include "Scierror.h"
#include "stricmp.h"
#include "freeArrayOfString.h"
#include "expandPathVariable.h"
#include "PATH_MAX.h"
#include "charEncoding.h"
#include "isdir.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int C2F(sci_getmd5) (char *fname,unsigned long fname_len)
{
	int m1 = 0, n1 = 0;

	int mn = 0;
	int i  = 0;
	
	char **Input_Matrix  = NULL;
	char **Output_Matrix = NULL;

	Rhs = Max(Rhs, 0);
	CheckRhs(1,2) ;
	CheckLhs(1,1) ;

	if (Rhs == 1)
	{
		if (GetType(1) == sci_strings)
		{
			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE, &m1, &n1, &Input_Matrix);
			mn = m1 * n1;

			Output_Matrix = (char**)MALLOC(sizeof(char*)*(mn));
			if (Output_Matrix)
			{
				for (i = 0; i < mn; i++)
				{
					FILE *fp = NULL;
					char *MD5 = NULL;
					char *real_path = NULL;

					/* Replaces SCI, ~, HOME, TMPDIR by the real path */
					real_path = expandPathVariable(Input_Matrix[i]);

					/* bug 4469 */
					if (isdir(real_path))
					{
						Scierror(999,_("%s: The file %s does not exist.\n"), fname, Input_Matrix[i]);
						freeArrayOfString(Output_Matrix, i);
						freeArrayOfString(Input_Matrix, mn);
						FREE(real_path); real_path = NULL;
						return 0;
					}

					wcfopen(fp, real_path, "rb");

					if (real_path) {FREE(real_path); real_path = NULL;}

					if (fp)
					{
						MD5 = md5_file(fp);
						fclose(fp);
						Output_Matrix[i] = strdup(MD5);
						if (MD5) {FREE(MD5);MD5 = NULL;}
					}
					else
					{
						Scierror(999, _("%s: The file %s does not exist.\n"), fname, Input_Matrix[i]);
						freeArrayOfString(Output_Matrix,i);
						freeArrayOfString(Input_Matrix, mn);
						return 0;
					}
				}

				CreateVarFromPtr( Rhs+1, MATRIX_OF_STRING_DATATYPE, &m1, &n1, Output_Matrix );
				LhsVar(1) = Rhs+1 ;
				PutLhsVar();
			}
			else
			{
				Scierror(999,_("%s: Memory allocation error.\n"), fname);
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong type of input argument #%d: String expected.\n"),fname,1);
		}
	}
	else /* Rhs == 2 */
	{
		if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
		{
			int m2 = 0, n2 = 0, l2 = 0;
			char *Param2 = NULL;

			GetRhsVar(1, MATRIX_OF_STRING_DATATYPE, &m1, &n1, &Input_Matrix);
			mn = m1 * n1;

			GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
			Param2 = cstk(l2);

			if ( stricmp(Param2, "string") == 0 )
			{
				Output_Matrix = (char**)MALLOC(sizeof(char*)*(mn));

				if (Output_Matrix)
				{
					for (i = 0; i < mn; i++)
					{
						char *MD5 = NULL;

						MD5 = md5_str(Input_Matrix[i]);
						Output_Matrix[i] = strdup(MD5);
						if (MD5) {FREE(MD5);MD5 = NULL;}

						if (Output_Matrix[i] == NULL)
						{
							freeArrayOfString(Input_Matrix, m1*n1);
							freeArrayOfString(Output_Matrix,i);
							Scierror(999,("%s: No more memory.\n"), fname);
							return 0;
						}
					}

					CreateVarFromPtr(Rhs + 1, MATRIX_OF_STRING_DATATYPE, &m1, &n1, Output_Matrix );
					LhsVar(1) = Rhs + 1 ;
					PutLhsVar();
				}
				else
				{
					Scierror(999,_("%s: Memory allocation error.\n"), fname);
				}
			}
			else
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: \"%s\" expected.\n"),fname,2,"string");
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input arguments #%d or #%d: Strings expected.\n"),fname,1,2);
		}
	}

	freeArrayOfString(Input_Matrix, mn);
	freeArrayOfString(Output_Matrix, mn);

	return 0;
}
/*--------------------------------------------------------------------------*/
