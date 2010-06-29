/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "function.hxx"
#include "context.hxx"
#include "types.hxx"
#include "double.hxx"

extern "C"
{
#include "MALLOC.h"
#include "getos.h"
#include "localization.h"
#include "Scierror.h"
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_getos(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
	char *OperatingSystem = getOSFullName();

    if(in.size() != 0)
    {
        Scierror(77,_("%s: Wrong number of input argument(s): %d expected.\n"), "getos", 0);
        return Function::Error;
    }

    if(_iRetCount != 2 && _iRetCount != 1 && _iRetCount != -1)
    {
        Scierror(78,_("%s: Wrong number of output argument(s): %d to %d expected.\n"), "getos", 1, 2);
        return Function::Error;
    }

	if (OperatingSystem)
	{
        String* pS1 = new String(OperatingSystem);
        out.push_back(pS1);
		if (OperatingSystem)
        {
            FREE(OperatingSystem);
            OperatingSystem = NULL;
        }


		if (_iRetCount == 2)
		{
			char *Release = getOSRelease();
			if (Release)
			{
                String* pS2 = new String(Release);
                out.push_back(pS2);
				if (Release) 
                {
                    FREE(Release);
                    Release = NULL;
                }
			}
			else
			{
				Scierror(999,_("%s: No more memory.\n"), "getos");
				return Function::Error;
			}
		}
	}
	else
	{
		Scierror(999,_("%s: No more memory.\n"), "getos");
        return Function::Error;
	}
	return Function::OK;
}
/*--------------------------------------------------------------------------*/
