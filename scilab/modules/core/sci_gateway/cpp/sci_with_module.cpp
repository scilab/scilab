/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "core_gw.hxx"
#include "funcmanager.hxx"
#include "function.hxx"
#include "string.hxx"
#include "bool.hxx"
#include "configvariable.hxx"


extern "C"
{
#include "charEncoding.h"
#include "Scierror.h"
#include "localization.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_with_module(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "with_module", 1);
        return types::Function::Error;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "with_module", 1);
        return types::Function::Error;
    }

    wchar_t* pwstModuleName = in[0]->getAs<types::String>()->get(0);

    types::Bool* pOut = new types::Bool(0);
    list<wstring> sModuleList = ConfigVariable::getModuleList();
    list<wstring>::iterator it;
    for (it = sModuleList.begin() ; it != sModuleList.end() ; it++)
    {
        if (*it == pwstModuleName)
        {
            pOut->get()[0] = 1;
            break;
        }
    }

    out.push_back(pOut);
    return types::Function::OK;
    //static int l1,n1,m1;
    //int *Status=NULL;

    //CheckRhs(1,1);
    //CheckLhs(1,1);

    //if (GetType(1) == sci_strings)
    //{
    //	char *VarName=NULL;

    //	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
    //	VarName=cstk(l1);

    //	Status=(int*)MALLOC(sizeof(int));
    //	*Status=with_module(VarName);

    //	m1=1;n1=1;
    //	CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &Status);
    //	LhsVar(1)=Rhs+1;

    //	C2F(putlhsvar)();

    //	if (Status) {FREE(Status);Status=NULL;}
    //}
    //else
    //{
    //	Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname,1);
    //	return 0;
    //}

    //return 0;
}
/*--------------------------------------------------------------------------*/


