/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2005-2008 - INRIA - Allan CORNET
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "Scierror.h"
#include "localization.h"
#include "TCL_ArrayExist.h"
#include "TCL_ArrayDim.h"
#include "TCL_ArrayGetVar.h"
#include "MALLOC.h"
#include "GlobalTclInterp.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_GetVar(char *fname,unsigned long l)
{
	static int l1 = 0,n1 = 0,m1 = 0;
	static int l2 = 0,n2 = 0,m2 = 0;

	Tcl_Interp *TCLinterpreter = NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char *VarName=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		VarName = cstk(l1);

		if (!existsGlobalInterp())
		{
			Scierror(999,_("%s: Error main TCL interpreter not initialized.\n"),fname);
			return 0;
		}

		if (Rhs == 2)
		{
			/* two arguments given - get a pointer on the slave interpreter */
			if (GetType(2) == sci_strings)
			{
				GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
				TCLinterpreter = Tcl_GetSlave(getTclInterp(),cstk(l2));
				if (TCLinterpreter == NULL)
				{
					Scierror(999,_("%s: No such slave interpreter.\n"),fname);
					return 0;
				}
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname, 2);
				return 0;
			}
		}
		else
		{
			/* only one argument given - use the main interpreter */
			TCLinterpreter = getTclInterp();
		}

		if (TCL_ArrayExist(TCLinterpreter,VarName))
		{
			int j = 0;
			int nb_lines=0, nb_columns=0;
			char **index_list = TCL_ArrayDim(TCLinterpreter,VarName, &nb_lines, &nb_columns);

			if (index_list)
			{
				char **ReturnArrayString = MALLOC(nb_lines * nb_columns * sizeof(char*));
				if (ReturnArrayString)
				{
					for (j = 0 ; j < nb_lines * nb_columns ; j++)
					{
						ReturnArrayString[j] = TCL_ArrayGetVar(TCLinterpreter, VarName, index_list[j]);
					}

					CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &nb_lines, &nb_columns, ReturnArrayString);
					LhsVar(1) = Rhs+1;
					
					freeArrayOfString(ReturnArrayString,nb_lines * nb_columns);
					freeArrayOfString(index_list,nb_lines * nb_columns);
                    PutLhsVar();
				}
				else
				{
					freeArrayOfString(index_list,nb_lines * nb_columns);
					Scierror(999,_("%s: No more memory.\n"),fname);
					return 0;
				}
			}
			else
			{
				Scierror(999,_("%s: No more memory.\n"),fname);
				return 0;
			}
		}
		else
		{
			char *RetStr = (char*)Tcl_GetVar(TCLinterpreter, VarName, TCL_GLOBAL_ONLY);
			if ( RetStr )
			{
				char *output = NULL ;

				output = strdup(RetStr);

				n1=1;
				CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,(m1=(int)strlen(output), &m1),&n1,&output);
                if (output) {FREE(output);output=NULL;}
				LhsVar(1) = Rhs+1;
				PutLhsVar();
			}
			else
			{
				releaseTclInterp();
				Scierror(999,_("%s: Could not read Tcl Variable.\n"),"TCL_GetVar");
				return 0;
			}
		}
	}
	else
    {
		releaseTclInterp();
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname, 1);
		return 0;
    }
	releaseTclInterp();
	return 0;
}
/*--------------------------------------------------------------------------*/
