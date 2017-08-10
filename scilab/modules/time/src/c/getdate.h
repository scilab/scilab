/*--------------------------------------------------------------------------*/

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * Copyright (C) DIGITEO - 2012 - Allan CORNET
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

/*--------------------------------------------------------------------------*/
#ifndef __GETDATE_H__
#define __GETDATE_H__

#include "dynlib_time.h"

#define NB_ELEMNT_ARRAY_GETDATE 10

/**
* Get Current Date as vector of double
* @param[out] ierr 0 eqs. OK
* return an array of double
*/
TIME_IMPEXP double *getCurrentDateAsDoubleVector(int *iErr);

/**
* Get Current Date as Unix Time convention
* return a double
*/
TIME_IMPEXP double getCurrentDateAsUnixTimeConvention(void);

/**
* Convert a double (Unix Time Convention) to an human date
* (a vector of double)
* @param[in] date as a double
* @param[out] ierr 0 eqs. OK
* return an array of double
*/
TIME_IMPEXP double *getConvertedDateAsDoubleVector(double dDate, int *iErr);

/**
* Convert doubles (Unix Time Convention) to an human dates
* (a matrix of double)
* @param[in] date as a double
* @param[out] ierr 0 eqs. OK
* return an array of double
*/
TIME_IMPEXP double *getConvertedDateAsMatrixOfDouble(double *dDates, int nbElements, int *iErr);

#endif /* __GETDATE_H__ */
