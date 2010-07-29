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
#include "funcmanager.hxx"
#include "context.hxx"
#include "core_gw.hxx"

extern "C"
{
#include "sci_mode.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_mode.h"
#include "os_strdup.h"
}
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_getscilabmode(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	int n1 = 0, m1 = 0;
	char *output = NULL ;

    if(in.size() != 0)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "getscilabmode", 0);
        return Function::Error;
    }

    String* pS = new String(getScilabModeString());
    out.push_back(pS);

    return Function::OK;
}
/*--------------------------------------------------------------------------*/
