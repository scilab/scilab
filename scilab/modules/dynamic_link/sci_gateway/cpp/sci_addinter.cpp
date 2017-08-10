/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*---------------------------------------------------------------------------*/
#include "dynamic_link_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "addinter.h"
#include "localization.h"
#include "Scierror.h"
#include "dl_genErrorMessage.h"
}
/*-----------------------------------------------------------------------------------*/
types::Function::ReturnValue sci_addinter(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iErr = 0;
    if (in.size() != 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "addinter", 3);
        return types::Function::Error;
    }

    //1st parameter
    if (in[0]->isString() == false)
    {
        Scierror(999 , _("%s : Wrong type for input argument #%d: string expected.\n"), "addinter", 1);
        return types::Function::Error;
    }

    types::String* pSLibName = in[0]->getAs<types::String>();
    if (pSLibName->isScalar() == false)
    {
        Scierror(999 , _("%s : Wrong type for input argument #%d: string expected.\n"), "addinter", 1);
        return types::Function::Error;
    }


    //2nd parameter
    if (in[1]->isString() == false)
    {
        Scierror(999 , _("%s : Wrong type for input argument #%d: string expected.\n"), "addinter", 2);
        return types::Function::Error;
    }

    types::String* pSModuleName = in[1]->getAs<types::String>();
    if (pSModuleName->isScalar() == false)
    {
        Scierror(999 , _("%s : Wrong type for input argument #%d: string expected.\n"), "addinter", 2);
        return types::Function::Error;
    }

    //3rd parameter
    if (in[2]->isString() == false)
    {
        Scierror(999 , _("%s : Wrong type for input argument #%d: string expected.\n"), "addinter", 3);
        return types::Function::Error;
    }

    types::String* pSFunctionList = in[2]->getAs<types::String>();
    if (pSFunctionList->isVector() == false)
    {
        Scierror(999 , _("%s : Wrong type for input argument #%d: String vector expected.\n"), "addinter", 3);
        return types::Function::Error;
    }

    iErr = AddInterfaceToScilab(pSLibName->get(0), pSModuleName->get(0), pSFunctionList->get(), pSFunctionList->getSize());
    if (iErr)
    {
        dl_genErrorMessage(L"addinter", iErr, pSLibName->get(0));
        return types::Function::Error;
    }

    return types::Function::OK;
    /*
    	CheckRhs(3,3);
    	CheckLhs(1,1);

    	if ( (VarType(1) == sci_strings) &&
    		(VarType(2) == sci_strings) &&
    		(VarType(3) == sci_strings) )
    	{

    		char **sharedlibname = NULL;
    		char **spname = NULL;
    		char **fcts = NULL;
    		int nbfcts = 0;
    		int ierr = 0;

    		int m1 = 0, n1 = 0;
    		int m2 = 0, n2 = 0;
    		int m3 = 0, n3 = 0;

    		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&sharedlibname);
    		if ( (m1 != n1) && (n1 != 1) )
    		{
    			freeArrayOfString(sharedlibname, m1*n1);
    			Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,1);
    			return 0;
    		}

    		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&spname);
    		GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&m3,&n3,&fcts);

    		if ( (m2 != n2) && (n2 != 1) )
    		{
    			freeArrayOfString(sharedlibname, m1*n1);
    			freeArrayOfString(spname, m2*n2);
    			freeArrayOfString(fcts, m3*n3);
    			Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,2);
    			return 0;
    		}

    		if ( (m3 > 1) &&  (n3 > 1) ) // check vector string
    		{
    			freeArrayOfString(sharedlibname, m1*n1);
    			freeArrayOfString(spname, m2*n2);
    			freeArrayOfString(fcts, m3*n3);
    			Scierror(999,_("%s: Wrong size for input argument #%d: String vector expected.\n"),fname,3);
    			return 0;
    		}

    		if ( (m3 == 1) && (n3 >= 1) )
    		{
    			nbfcts = n3;
    		}
    		else if ( (n3 == 1) && (m3 >= 1) )
    		{
    			nbfcts = m3;
    		}

    		if (nbfcts >= NumberMaxFunctionsByGateway)
    		{
    			freeArrayOfString(sharedlibname, m1*n1);
    			freeArrayOfString(spname, m2*n2);
    			freeArrayOfString(fcts, m3*n3);
    			Scierror(999,_("%s: Wrong size for input argument #%d: String vector < %d expected.\n"),fname,3,NumberMaxFunctionsByGateway);
    			return 0;
    		}

    		ierr = AddInterfaceToScilab(sharedlibname[0],spname[0],fcts,nbfcts);
    		if (ierr == 0)
    		{
    			LhsVar(1) = 0;
                PutLhsVar();
    		}
    		else
    		{
    			dl_genErrorMessage(fname, ierr, sharedlibname[0]);
    		}

    		freeArrayOfString(sharedlibname, m1*n1);
    		freeArrayOfString(spname, m2*n2);
    		freeArrayOfString(fcts, m3*n3);
    	}
    	else
    	{
    		Scierror(999,_("%s: Wrong type for input arguments: Strings expected.\n"),fname);
    	}
    	return 0;
    */
}
/*---------------------------------------------------------------------------*/
