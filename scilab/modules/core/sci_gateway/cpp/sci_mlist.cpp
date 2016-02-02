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
#include "mlist.hxx"
#include "function.hxx"
#include "sci_tlist_or_mlist.hxx"

types::Function::ReturnValue sci_mlist_gw(types::typed_list &in, int _piRetCount, types::typed_list &out)
{
    return sci_tlist_or_mlist<types::MList>(in, _piRetCount, out, L"mlist");
}
