/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2012 - DIGITEO - cedric delamarre
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

#ifndef __PROD_H__
#define __PROD_H__

#include "double.hxx"
#include "polynom.hxx"

extern "C"
{
#include "dynlib_elementary_functions.h"
}

ELEMENTARY_FUNCTIONS_IMPEXP types::Double*  prod(types::Double*  pIn, int iOrientation);
ELEMENTARY_FUNCTIONS_IMPEXP types::Polynom* prod(types::Polynom* pIn, int iOrientation);

#endif /* __PROD_H__ */
