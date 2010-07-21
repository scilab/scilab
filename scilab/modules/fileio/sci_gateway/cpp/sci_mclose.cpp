/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 * 
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 * 
 */

#include "alltypes.hxx"
#include "funcmanager.hxx"
#include "filemanager.hxx"
#include "fileio_gw.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "mclose.h"
#include "os_wcsicmp.h"
}

using namespace types;

Function::ReturnValue sci_mclose(typed_list &in, int _iRetCount, typed_list &out)
{
    int iRet = 0;
    if(in.size() == 0)
    {//close current file
        iRet = mcloseCurrentFile();
    }
    else if(in.size() == 1)
    {
        if(in[0]->getType() == InternalType::RealString)
        {
            String *pS = in[0]->getAsString();
            if(pS->size_get() != 1)
            {
                ScierrorW(999,_W("%ls: Wrong type for input argument #%d: A String expected.\n"), L"mclose", 1);
                return Function::Error;
            }

            if(os_wcsicmp(pS->string_get(0), L"all") != 0)
            {
                ScierrorW(999, _W("%ls: Wrong input arguments: '%ls' expected.\n"), L"mclose", L"all");
                return Function::Error;
            }

            iRet = mcloseAll();
        }
        else if(in[0]->getType() == InternalType::RealDouble)
        {
            Double* pD = in[0]->getAsDouble();
            if(pD->size_get() != 1 || pD->isComplex())
            {
                ScierrorW(999,_W("%ls: Wrong type for input argument #%d: A real expected.\n"), L"mclose", 1);
                return Function::Error;
            }

            int iVal = static_cast<int>(pD->real_get()[0]);
            iRet = mclose(iVal);
        }
        else
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A integer or string expected.\n"), L"mclose", 1);
            return Function::Error;
        }
    }
    else
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d or %d expected.\n"), L"mclose", 0, 1);
        return Function::Error;
    }

    Double* pD = new Double(static_cast<double>(iRet));
    out.push_back(pD);
    return Function::OK;
}

int sci_mclose(char *fname,unsigned long fname_len)
{
	//int m1 = 0,n1 = 0,l1 =0;
	//int one = 1, l2 = 0;
	//int fd = ALL_FILES_DESCRIPTOR;

	//Nbvars = 0;
	//CheckRhs(0,1);
	//CheckLhs(1,1);

	//if (Rhs == 0)
	//{
	//	fd = ALL_FILES_DESCRIPTOR;
	//	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &l2);

	//	C2F(mclose)(&fd,stk(l2));

	//	LhsVar(1) = Rhs+1;
	//	PutLhsVar();
	//}
	//else /* Rhs == 1 */
	//{
	//	if (GetType(1) == sci_matrix)
	//	{
	//		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
	//		if (m1*n1 == 1)
	//		{
	//			fd = *istk(l1);
	//			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &l2);

	//			C2F(mclose)(&fd,stk(l2));

	//			LhsVar(1) = Rhs+1;
	//			PutLhsVar();
	//		}
	//		else
	//		{
	//			Scierror(999, _("%s: Wrong size for input argument #%d: A integer expected.\n"), fname,1);
	//		}
	//	}
	//	else if (GetType(1) == sci_strings)
	//	{
	//		char **input_string = NULL;

	//		GetRhsVar(1 ,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&input_string);
	//		if (m1*n1 == 1)
	//		{
	//			double err = 0.0;

	//			if (strcmp(input_string[0],"all") == 0)
	//			{
	//				fd = ALL_FILES_DESCRIPTOR;
	//			}
	//			else
	//			{
	//				fd = GetIdFromFilename(input_string[0]);
	//			}

	//			if ( (fd == FILE_ID_NOT_DEFINED) && getWarningMode() )
	//			{
	//				sciprint(_("%s: No such file %s.\n"),fname, input_string[0]);
	//			}
	//			else
	//			{
	//				C2F(mclose)(&fd,&err);
	//			}

	//			freeArrayOfString(input_string,m1*n1);

	//			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &one, &one, &l2);
	//			*stk(l2) = err;

	//			LhsVar(1) = Rhs+1;
	//			PutLhsVar();
	//		}
	//		else
	//		{
	//			freeArrayOfString(input_string,m1*n1);
	//			Scierror(999, _("%s: Wrong size for input argument #%d: A string expected.\n"), fname,1);
	//		}
	//	}
	//	else
	//	{
	//		Scierror(999, _("%s: Wrong type for input argument #%d: A integer or string expected.\n"), fname,1);
	//	}
	//}
	return 0;
}
/*--------------------------------------------------------------------------*/
