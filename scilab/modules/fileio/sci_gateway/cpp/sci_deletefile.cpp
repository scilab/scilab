/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2010 - INRIA - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "filemanager.hxx"
#include "fileio_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "bool.hxx"

extern "C"
{
#include "Scierror.h"
#include "deleteafile.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_deletefile(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "deletefile", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "deletefile", 1);
        return types::Function::Error;
    }


    types::Bool *pOut = NULL;
    if (deleteafileW(in[0]->getAs<types::String>()->get(0)))
    {
        pOut = new types::Bool(1);
    }
    else
    {
        pOut = new types::Bool(0);
    }

    out.push_back(pOut);
    return types::Function::OK;
    //CheckRhs(1,1);
    //CheckLhs(1,1);

    //if (GetType(1) == sci_strings)
    //{
    //	int m1,n1,l1;
    //	char *VarName=NULL;

    //	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
    //	/* Bug 3089 */
    //	VarName = cstk(l1);

    //	n1=1;
    //	if ( deleteafile(VarName) )
    //	{
    //		CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
    //		*istk(l1)=(int)(TRUE);
    //	}
    //	else
    //	{
    //		CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
    //		*istk(l1)=(int)(FALSE);
    //	}

    //	LhsVar(1)=Rhs+1;
    //	C2F(putlhsvar)();
    //}
    //else
    //{
    //	Scierror(999,_("%s: Wrong type for input argument: A string expected.\n"),fname);
    //}
    //return 0;
}
/*--------------------------------------------------------------------------*/
