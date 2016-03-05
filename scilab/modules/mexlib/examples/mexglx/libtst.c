/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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

#include <mex.h>
extern Gatefunc mexFunction;
static GenericTable Tab[] =
{
    {mex_gateway, mexFunction, "errmsg"},
};

int C2F(libtst)()
{
    Rhs = Max(0, Rhs);
    (*(Tab[Fin - 1].f))(Tab[Fin - 1].name, Tab[Fin - 1].F);
    return 0;
}
