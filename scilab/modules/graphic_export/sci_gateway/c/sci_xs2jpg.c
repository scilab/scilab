/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Koumar Sylvestre
 * desc : interface for xs2jpg routine
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

#include "gw_graphic_export.h"
#include "xs2file.h"

/*--------------------------------------------------------------------------*/
int sci_xs2jpg(char * fname, void* pvApiCtx)
{
    return xs2file(fname, JPG_EXPORT, pvApiCtx);
}
/*--------------------------------------------------------------------------*/
