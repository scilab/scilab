/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric Delamarre
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __DMP2PM_H__
#define __DMP2PM_H__

#include "dynlib_cacsd.h"
// converted a matrix of polynom to a polynomial matrix
CACSD_IMPEXP double* dmp2pm(double** _pdblMP, int _iSizeMP, int* _piRanks, int _iMaxRank);

#endif /* __DMP2PM_H__ */
