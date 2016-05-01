/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
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

#ifndef __CLEAN_H__
#define __CLEAN_H__

extern "C"
{
#include "dynlib_elementary_functions.h"
}

ELEMENTARY_FUNCTIONS_IMPEXP void clean(double* pdblReal, double* pdblImg, int iSize, double dEpsA, double dEpsR);

#endif /* __CLEAN_H__ */
