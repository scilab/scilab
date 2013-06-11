/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "funcmanager.hxx"
#include "jvm_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include <string.h>
#include "system_getproperty.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_mode.h"
}
/*--------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_system_getproperty(typed_list &in, int _piRetCount, typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: at least %d expected.\n"), "system_getproperty", 1);
        return Function::Error;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "system_getproperty" , 1);
        return Function::Error;
    }

    if (getScilabMode() == SCILAB_NWNI)
    {
        Scierror(999, _("JVM interface disabled in -nogui or -nwni modes.\n"));
        return Function::Error;
    }

    char* pstProperty   = wide_string_to_UTF8(in[0]->getAs<types::String>()->get(0));
    char* pstValue      = system_getproperty(pstProperty, "unknown");
    String* pS          = new String(pstValue);
    out.push_back(pS);

    return Function::OK;
    //Rhs = Max(Rhs,0);
    //CheckRhs(1,1);
    //CheckLhs(0,1);

    //if ( GetType(1) == sci_strings )
    //{
    //	static int m1,n1=0,l1=0;
    //	char *propertyName = NULL;
    //	char *propertyValue = NULL;

    //	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
    //	propertyName=cstk(l1);
    //	propertyValue=system_getproperty(propertyName,"unknown");

    //	n1=1;
    //	CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m1=(int)strlen(propertyValue), &m1),&n1,&propertyValue);
    //	LhsVar(1)=Rhs+1;
    //	C2F(putlhsvar)();
    //	if (propertyValue) {FREE(propertyValue);propertyValue=NULL;}
    //}
    //else
    //{
    //	Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname, 1);
    //	return 0;
    //}
    //return 0;
}

/*--------------------------------------------------------------------------*/

