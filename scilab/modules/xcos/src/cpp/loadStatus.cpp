/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2014 - Clement DAVID <clement.david@scilab-enterprises.com>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "loadStatus.hxx"

static xcos_status_t status;

xcos_status_t get_loaded_status ()
{
    return status;
}

void set_loaded_status(xcos_status_t s)
{
    status = s;
}

