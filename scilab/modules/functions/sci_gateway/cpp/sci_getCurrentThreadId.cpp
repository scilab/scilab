/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "Thread_Wrapper.h"
#include "Scierror.h"
#include "localization.h"
}

#include "functions_gw.hxx"
#include "threadId.hxx"
#include "funcmanager.hxx"

using namespace types;

Function::ReturnValue sci_getCurrentThreadId(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	if(in.size() != 0)
	{
        ScierrorW(999, _W("%ls: Wrong number of input arguments: %d expected.\n"), L"getCurrentThreadId" , 0);
        return Function::Error;
	}

	if(_iRetCount != 1)
	{
        ScierrorW(999, _W("%ls: Wrong number of output arguments: %d expected.\n"), L"getCurrentThreadId" , 1);
        return Function::Error;
    }

    out.push_back(ThreadId::createThreadId(__GetCurrentThreadId()));

    return Function::OK;
}
