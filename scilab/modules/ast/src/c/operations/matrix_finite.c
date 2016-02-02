/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Antoine ELIAS <antoine.elias@scilab.org>
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

#include "finite.h"
#include "core_math.h"

/*return 1 if all is OK else 0*/
int matrix_finite(double *_pdbl, int _iSize)
{
    int i = 0;
    for (i = 0 ; i < _iSize ; i++)
    {
        if (finite(_pdbl[i]) == 0)
        {
            return 1;
        }
    }
    return 0;
}
