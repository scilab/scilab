/*--------------------------------------------------------------------------*/

/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 * Copyright (C) DIGITEO - 2012 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GETDATE_H__
#define __GETDATE_H__

#define NB_ELEMNT_ARRAY_GETDATE 10

/**
* Get Current Date as vector of double
* @param[out] ierr 0 eqs. OK
* return an array of double
*/
double *getCurrentDateAsDoubleVector(int *iErr);

/**
* Get Current Date as Unix Time convention
* return a double
*/
double getCurrentDateAsUnixTimeConvention(void);

/**
* Convert a double (Unix Time Convention) to an human date
* (a vector of double)
* @param[in] date as a double
* @param[out] ierr 0 eqs. OK
* return an array of double
*/
double *getConvertedDateAsDoubleVector(double dDate, int *iErr);

/**
* Convert doubles (Unix Time Convention) to an human dates
* (a matrix of double)
* @param[in] date as a double
* @param[out] ierr 0 eqs. OK
* return an array of double
*/
double *getConvertedDateAsMatrixOfDouble(double *dDates, int nbElements, int *iErr);

#endif /* __GETDATE_H__ */
