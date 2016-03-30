/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) Scilab Enterprises - 2014 - Clement DAVID <clement.david@scilab-enterprises.com>
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

