/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "libraryinfo.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_libraryinfo)(char *fname,unsigned long fname_len)
{
	int l1,n1,m1;

	CheckRhs(1,1);
	CheckLhs(1,2);
	
	if (GetType(1) == sci_strings)
	{
		char *pathlibrary = NULL;
		char *libraryname = NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		libraryname = cstk(l1);

		pathlibrary = getlibrarypath(libraryname);

		if (pathlibrary)
		{
			int m = 0, n = 0, l = 0;
			char **macros = NULL;
			int sizemacrosarray = 0;

			macros = getlistmacrosfromlibrary(libraryname,&sizemacrosarray);

			if (macros)
			{
				m = sizemacrosarray;
				n = 1;
				CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &m, &n, macros);
			}
			else
			{
				n = 0;
				m = 0;
				l = 0;
				CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&n,&m,&l);
			}
			LhsVar(1) = Rhs+1;

			if (macros)
			{
				int i = 0;
				for( i = 0; i < sizemacrosarray; i++)
				{
					if (macros[i]) 
					{
						FREE(macros[i]);
						macros[i] = NULL;
					}
				}
				FREE(macros);
				macros = NULL;
			}

			if (Lhs == 2)
			{
				n = 1;
				m = (int)strlen(pathlibrary);
				CreateVarFromPtr(Rhs+2,STRING_DATATYPE,&m,&n,&pathlibrary);
				LhsVar(2) = Rhs+2;
				if (pathlibrary) {FREE(pathlibrary);pathlibrary=NULL;}
			}

			C2F(putlhsvar)();
		}
		else
		{
			Scierror(999,_("%s: Invalid library %s.\n"),fname, libraryname);
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type of input argument #%d: String expected.\n"),fname,1);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
