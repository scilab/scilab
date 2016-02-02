/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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
#include "listinsert.hxx"
#include "listundefined.hxx"

types::Function::ReturnValue sci_insert(types::typed_list &in, int _piRetCount, types::typed_list &out)
{
    if (in.size() == 0)
    {
        out.push_back(new types::ListInsert(new types::ListUndefined()));
    }
    else
    {
        out.push_back(new types::ListInsert(in[0]));
    }
    return types::Function::OK;
}
