/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "types_gw.hxx"
#include "mlist.hxx"
#include "function.hxx"
#include "sci_tlist_or_mlist.hxx"

types::Function::ReturnValue sci_mlist(types::typed_list &in, int _piRetCount, types::typed_list &out)
{
    return sci_tlist_or_mlist<types::MList>(in, _piRetCount, out, L"mlist");
}
