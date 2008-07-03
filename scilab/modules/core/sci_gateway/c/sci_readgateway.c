/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "gw_core.h"
#include "stack-c.h"
#include "readGateway.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "with_module.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_readgateway)(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(1,3);

	if (VarType(1) == sci_strings)
	{
		int l1 = 0,n1 = 0,m1 = 0;
		char *moduleName = NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		moduleName = cstk(l1);

		if ( with_module(moduleName) )
		{
			struct gateway_struct *gateway_content = NULL;

			gateway_content = readGateway(moduleName);

			if (gateway_content)
			{
				if (gateway_content->dimLists == 0)
				{
					m1 = 0; n1 = 0; l1 = 0;
					CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
					LhsVar(1) = Rhs + 1;

					if (Lhs == 2)
					{
						m1 = 0; n1 = 0; l1 = 0;
						CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
						LhsVar(2) = Rhs + 2;
					}

					if (Lhs == 3)
					{
						m1 = 0; n1 = 0; l1 = 0;

						CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
						LhsVar(2) = Rhs + 2;

						CreateVar(Rhs+3,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
						LhsVar(3) = Rhs + 3;
					}
				}
				else
				{
					int *output = NULL;
					m1 = gateway_content->dimLists;
					n1 = 1;

					CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,gateway_content->primitivesList);
					LhsVar(1) = Rhs + 1;

					if (Lhs == 2)
					{
						output = gateway_content->primiviteIdList;
						CreateVarFromPtr(Rhs+2,MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &output);
						LhsVar(2) = Rhs + 2;
					}
					
					if (Lhs == 3)
					{
						output = gateway_content->primiviteIdList;
						CreateVarFromPtr(Rhs+2,MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &output);
						LhsVar(2) = Rhs + 2;

						output = gateway_content->gatewayIdList;
						CreateVarFromPtr(Rhs+3,MATRIX_OF_INTEGER_DATATYPE, &m1, &n1, &output);
						LhsVar(3) = Rhs + 3;
					}

					/* FREE struct */
					freeArrayOfString(gateway_content->primitivesList,gateway_content->dimLists);
					if (gateway_content->primiviteIdList)
					{
						FREE(gateway_content->primiviteIdList);
						gateway_content->primiviteIdList = NULL;
					}
					if (gateway_content->gatewayIdList)
					{
						FREE(gateway_content->gatewayIdList);
						gateway_content->gatewayIdList = NULL;
					}
					FREE(gateway_content);
					gateway_content = NULL;
				}
				C2F(putlhsvar)();
			}
			else
			{
				Scierror(999,_("%s: Impossible to read %s gateway.\n"),fname,moduleName);
			}
		}
		else
		{
			Scierror(999,_("%s: Invalid module name : %s.\n"),fname,moduleName);
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname, 1);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
