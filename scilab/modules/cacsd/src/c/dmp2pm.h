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

#ifndef __DMP2PM_H__
#define __DMP2PM_H__

#include "dynlib_cacsd.h"
// converted a matrix of polynom to a polynomial matrix
CACSD_IMPEXP double* dmp2pm(double** _pdblMP, int _iSizeMP, int* _piRanks, int _iMaxRank);

#endif /* __DMP2PM_H__ */
