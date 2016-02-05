/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include "core_gw.hxx"
#include "function.hxx"
#include "callable.hxx"
#include "context.hxx"
#include "string.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
}

types::Function::ReturnValue sci_clear(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    symbol::Context* ctx = symbol::Context::getInstance();
    int iWrongType = 1;

    if (in.size() == 0)
    {
        //clear();
        ctx->removeAll();
        return types::Function::OK;
    }

    // First check if all arguments are Single Strings.
    int var = 0;
    for (auto pIT : in)
    {
        ++var;
        if (pIT->isString() == false)
        {
            Scierror(207, _("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"), "clear", var);
            return types::Function::Error;
        }
    }

    bool bShow = false;
    for (auto pIT : in)
    {
        types::String* pStr = pIT->getAs<types::String>();
        for (int i = 0; i < pStr->getSize(); i++)
        {
            symbol::Symbol sym = symbol::Symbol(pStr->get(i));
            if (ctx->isprotected(sym) == false)
            {
                ctx->remove(sym);
            }
            else
            {
                if (bShow == false)
                {
                    Scierror(999, _("Redefining permanent variable.\n"));
                    bShow = true;
                }
            }
        }
    }

    if (bShow)
    {
        return types::Function::Error;
    }

    return types::Function::OK;
}
