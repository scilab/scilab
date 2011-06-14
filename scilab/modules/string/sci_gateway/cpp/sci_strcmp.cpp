/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Digiteo 2011 - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "function.hxx"
#include "string.hxx"
#include "list.hxx"
#include "double.hxx"
#include "function.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
#include "stringsCompare.h"
}

#define CHAR_I 'i'
#define CHAR_S 's'

types::Function::ReturnValue sci_strcmp(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pOutDouble   = NULL;
    types::String* pString1     = NULL;
    types::String* pString2     = NULL;
    wchar_t* pwcChar3           = NULL;
    BOOL do_stricmp             = FALSE;

    if(in.size() < 2 || in.size() > 3)
    {
        ScierrorW(77, _W("%ls: Wrong number of input argument(s): %d to %d expected.\n"), L"strcmp", 2, 3);
        return types::Function::Error;
    }
    if(_iRetCount != 1)
    {
        ScierrorW(78, _W("%ls: Wrong number of output argument(s): %d expected.\n"), L"strcmp", 1);
        return types::Function::Error;
    }
	if(in[0]->isString() == false)
	{
		ScierrorW(999,_W("%ls: Wrong type for input argument #%d: String expected.\n"),L"strcmp", 1);
		return types::Function::Error;
	}
	if(in[1]->isString() == false)
	{
		ScierrorW(999,_W("%ls: Wrong type for input argument #%d: String expected.\n"),L"strcmp", 2);
		return types::Function::Error;
	}

    pString1 = in[0]->getAs<types::String>();
    pString2 = in[1]->getAs<types::String>();



    if(pString1->getSize() != pString2->getSize() && pString2->isScalar() == false)
    {
        ScierrorW(999,_W("%ls: Wrong size for input argument #%d.\n"),L"strcmp", 2);
        return types::Function::Error;
    }

    if(in.size() == 3)
    {
        if(in[2]->isString() == false || in[2]->getAs<types::String>()->isScalar() == false || wcslen(in[2]->getAs<types::String>()->get(0)) != 1)
	    {
		    ScierrorW(999,_W("%ls: Wrong type for input argument #%d: Char expected.\n"),L"strcmp", 3);
		    return types::Function::Error;
	    }

	    pwcChar3 = in[2]->getAs<types::String>()->get(0);
	    if ( (pwcChar3[0] != CHAR_I) && (pwcChar3[0] != CHAR_S))
		{
			ScierrorW(999,_W("%ls: Wrong value for input argument #%d: %s or %s expected.\n"),L"strcmp", 3,"'i' (stricmp)","'s' (strcmp)");
			return types::Function::Error;
		}

        if (pwcChar3[0] == CHAR_I) do_stricmp = TRUE;
    }

	int *values = stringsCompare(pString1->get(),pString1->getSize(),pString2->get(),pString2->getSize(),do_stricmp);

	if (values)
	{
        pOutDouble  = new types::Double(pString1->getDims(), pString1->getDimsArray());
        pOutDouble->setInt(values);
    }
    else
    {
        ScierrorW(999,_W("%ls : No more memory.\n"),L"strcmp");
    }


    out.push_back(pOutDouble);
    return types::Function::OK;
}

