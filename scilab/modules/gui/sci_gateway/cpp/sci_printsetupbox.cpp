/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* Copyright (C) 2008 - INRIA - Vincent COUVERT (Java version)
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include "CallScilabBridge.hxx"
#include "GiwsException.hxx"

extern "C"
{
#include "MALLOC.h"
#include "stack-c.h"
#include "localization.h"
#include "getScilabJavaVM.h"
#include "Scierror.h"
#include "gw_gui.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_gui_bridge;

/*--------------------------------------------------------------------------*/
int sci_printsetupbox(char *fname, unsigned long l)
{
    static int n1 = 0;
    int *paramoutINT = new int[1];

    CheckRhs(0, 0);
    CheckLhs(0, 1);

    try
    {
        paramoutINT[0] = (int)CallScilabBridge::pageSetup(getScilabJavaVM());
    }
    catch(const GiwsException::JniException & e)
    {
        Scierror(999, _("%s: A Java exception arisen:\n%s"), fname, e.whatStr().c_str());
        return FALSE;
    }

    n1 = 1;
    CreateVarFromPtr(Rhs + 1, MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &paramoutINT);
    LhsVar(1) = Rhs + 1;
    delete[]paramoutINT;
    PutLhsVar();

    return TRUE;
}

/*--------------------------------------------------------------------------*/
