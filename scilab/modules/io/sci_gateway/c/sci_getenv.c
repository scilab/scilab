/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include "gw_io.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "Scierror.h"
#include "stack-c.h"
#include "getenvc.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getenv)(char *fname,unsigned long fname_len)
{
	char **default_env_value = NULL;
	int ierr = 0;
	
	char *env_value = NULL;
	int length_env = 0;

	char **env_name = NULL;

	Rhs = Max(Rhs,0);

	CheckRhs(1,2);
	CheckLhs(1,1);

	if (Rhs == 2)
	{
		if (GetType(2) == sci_strings)	
		{
			int m2 = 0, n2 = 0;
			int m2n2 = 0;

			GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&default_env_value);
			m2n2 = m2*n2;
			if (m2n2 != 1)
			{
				freeArrayOfString(default_env_value, m2n2);
				Scierror(999,_("%s: Wrong size for input argument #%d: String expected.\n") ,fname,2);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n") ,fname,2);
			return 0;
		}
	}

	if (GetType(1) == sci_strings)	
	{
		int m1 = 0, n1 = 0;
		int m1n1 = 0;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&env_name);
		m1n1 = m1*n1;
		if (m1n1 != 1)
		{
			freeArrayOfString(env_name, m1n1);
			freeArrayOfString(default_env_value, 1);
			Scierror(999,_("%s: Wrong size for input argument #%d: String expected.\n") ,fname,1);
			return 0;
		}
	}
	else
	{
		freeArrayOfString(default_env_value, 1);
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n") ,fname,1);
		return 0;
	}

	#ifdef _MSC_VER
	length_env = _MAX_ENV;
	#else
	length_env = bsiz;
	#endif
	env_value = (char*)MALLOC( (length_env + 1) *sizeof(char) );

	if (env_value)
	{
		int iflag = 0;
		C2F(getenvc)(&ierr, env_name[0], env_value, &length_env, &iflag);
		if (ierr == 0)
		{
			int n = 1; 
			int m = (int)strlen(env_value);

			CreateVarFromPtr(Rhs + 1,STRING_DATATYPE,&m,&n,&env_value);
			LhsVar(1) = Rhs + 1;
			C2F(putlhsvar)();	
		}
		else
		{
			if (default_env_value)
			{
				int n = 1; 
				int m = (int)strlen(default_env_value[0]);

				CreateVarFromPtr(Rhs + 1,STRING_DATATYPE,&m,&n,&default_env_value[0]);
				LhsVar(1) = Rhs + 1;
				C2F(putlhsvar)();	
			}
			else
			{
				Scierror(999,_("%s: Undefined environment variable %s.\n"), fname, env_name[0]);
			}
		}
	}
	else
	{
		Scierror(999,_("%s: No more memory.\n"), fname);
	}

	if (env_value) {FREE(env_value); env_value = NULL;}
	freeArrayOfString(env_name, 1);
	freeArrayOfString(default_env_value, 1);

	return 0;
}
/*--------------------------------------------------------------------------*/
