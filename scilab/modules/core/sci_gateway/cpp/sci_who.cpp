/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *  Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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


#include <sstream>
#include "core_gw.hxx"
#include "function.hxx"
#include "context.hxx"
#include "scilabWrite.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_who(types::typed_list& in, int _iRetCount, types::typed_list& out)
{
    std::string cWhat("");
    bool bSorted = false;
    types::String* pStrOut = NULL;
    types::Double* pDblOut = NULL;
    std::list<std::string> lstVar;
    int size = 0;

    if (in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "who", 0, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "who", 0, 2);
        return types::Function::Error;
    }

    if (in.size() == 0)
    {
        std::stringstream stream;
        symbol::Context::getInstance()->print(stream, bSorted);
        stream << std::endl;
        scilabForcedWrite(stream.str().c_str());
        return types::Function::OK;
    }

    if (in.size() == 2)
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "who", 2);
            return types::Function::Error;
        }

        types::String* pStrSorted = in[1]->getAs<types::String>();

        if (pStrSorted->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "who", 2);
            return types::Function::Error;
        }

        if (strcmp(pStrSorted->get(0), "sorted") == 0)
        {
            bSorted = true;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 'sorted' expected.\n"), "who", 2);
            return types::Function::Error;
        }
    }

    if (in.size() > 0)
    {
        if (in[0]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "who", 1);
            return types::Function::Error;
        }

        types::String* pStrWhat = in[0]->getAs<types::String>();

        if (pStrWhat->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A single string expected.\n"), "who", 1);
            return types::Function::Error;
        }

        cWhat = pStrWhat->get(0);
    }

    if (cWhat == "local" || cWhat == "get")
    {
        size = symbol::Context::getInstance()->getVarsNameForWho(lstVar, bSorted);
    }
    else if (cWhat == "global")
    {
        size = symbol::Context::getInstance()->getGlobalNameForWho(lstVar, bSorted);
    }
    else if (bSorted == false && cWhat == "sorted")
    {
        bSorted = true;
        std::stringstream stream;
        symbol::Context::getInstance()->print(stream, bSorted);
        stream << std::endl;
        scilabForcedWrite(stream.str().c_str());
        return types::Function::OK;
    }
    else
    {
        if (bSorted)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 'local', 'get' or 'global' expected.\n"), "who", 1);
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 'local', 'get', 'global' or 'sorted' expected.\n"), "who", 1);
        }

        return types::Function::Error;
    }

    if (lstVar.empty())
    {
        out.push_back(types::Double::Empty());
        if (_iRetCount == 2)
        {
            out.push_back(types::Double::Empty());
        }

        return types::Function::OK;
    }

    pStrOut = new types::String(size, 1);
    int i = 0;
    for (auto it : lstVar)
    {
        pStrOut->set(i++, it.c_str());
    }

    out.push_back(pStrOut);

    if (_iRetCount == 2)
    {
        pDblOut = new types::Double(pStrOut->getDims(), pStrOut->getDimsArray());
        memset(pDblOut->get(), 0x00, pDblOut->getSize() * sizeof(double));
        out.push_back(pDblOut);
    }

    return types::Function::OK;
}
