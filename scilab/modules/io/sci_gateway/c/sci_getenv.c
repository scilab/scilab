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
#include "api_scilab.h"
#include "getenvc.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getenv)(char *fname,unsigned long fname_len)
{
	StrErr strErr;
	int ierr = 0;
	char *default_env_value = NULL;
	char *env_value = NULL;
	int length_env = 0;
	char *env_name = NULL;

	int m1 = 0, n1 = 0;
	int *piAddressVarOne = NULL;
	int iType1	= 0;
	char *pStVarOne = NULL;
	int lenStVarOne = 0;

	int m2 = 0, n2 = 0;
	int *piAddressVarTwo = NULL;
	int iType2	= 0;
	char *pStVarTwo = NULL;
	int lenStVarTwo = 0;

	Rhs = Max(Rhs,0);

	CheckRhs(1,2);
	CheckLhs(1,1);

	if (Rhs == 2)
	{
		strErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		strErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if (iType2  != sci_strings )
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
			return 0;
		}

		strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if ( (m2 != n2) && (n2 != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
			return 0;
		}

		pStVarTwo = (char*)MALLOC(sizeof(char)*(lenStVarTwo + 1));
		if (pStVarTwo)
		{
			strErr = getMatrixOfString(pvApiCtx, piAddressVarTwo,&m2,&n2,&lenStVarTwo,&pStVarTwo);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return 0;
		}
	}

	strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	strErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if (iType1  != sci_strings )
	{
		if (pStVarTwo) {FREE(pStVarTwo); pStVarTwo = NULL;}
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
		return 0;
	}

	strErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if ( (m1 != n1) && (n1 != 1) ) 
	{
		if (pStVarTwo) {FREE(pStVarTwo); pStVarTwo = NULL;}
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
		return 0;
	}

	pStVarOne = (char*)MALLOC(sizeof(char)*(lenStVarOne + 1));
	if (pStVarOne)
	{
		strErr = getMatrixOfString(pvApiCtx, piAddressVarOne,&m1,&n1,&lenStVarOne,&pStVarOne);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}
	}
	else
	{
		if (pStVarTwo) {FREE(pStVarTwo); pStVarTwo = NULL;}
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
		return 0;
	}

	#ifdef _MSC_VER
	length_env = _MAX_ENV;
	#else
	length_env = bsiz;
	#endif

	default_env_value =  pStVarTwo;
	env_name = pStVarOne;

	env_value = (char*)MALLOC( (length_env + 1) *sizeof(char) );

	if (env_value == NULL)
	{
		if (default_env_value) {FREE(default_env_value); default_env_value = NULL;}
		if (env_name) {FREE(env_name); env_name = NULL;}

		Scierror(999,_("%s: No more memory.\n"), fname);
		return 0;
	}
	else
	{
		int m_out = 1, n_out = 1;
		int iflag = 0;

		C2F(getenvc)(&ierr, env_name, env_value, &length_env, &iflag);

		if (ierr == 0)
		{
			strErr = createMatrixOfString(pvApiCtx, Rhs + 1, m_out, n_out, &env_value);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			LhsVar(1) = Rhs + 1;
			C2F(putlhsvar)();	
		}
		else
		{
			if (default_env_value)
			{
				strErr = createMatrixOfString(pvApiCtx, Rhs + 1, m_out, n_out, &default_env_value);
				if(strErr.iErr)
				{
					printError(&strErr, 0);
					return 0;
				}

				LhsVar(1) = Rhs + 1;
				C2F(putlhsvar)();	
			}
			else
			{
				Scierror(999,_("%s: Undefined environment variable %s.\n"), fname, env_name);
			}
		}

		if (default_env_value) {FREE(default_env_value); default_env_value = NULL;}
		if (env_name) {FREE(env_name); env_name = NULL;}

	}
	return 0;
}
/*--------------------------------------------------------------------------*/
