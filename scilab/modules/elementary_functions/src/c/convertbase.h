/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __CONVERTBASE_H__
#define __CONVERTBASE_H__

typedef enum
{
    ERROR_CONVERTBASE_OK = 0,
    ERROR_CONVERTBASE_NOK = 1,
    ERROR_CONVERTBASE_NOT_INTEGER_VALUE = 2,
    ERROR_CONVERTBASE_NOT_IN_INTERVAL = 3,
    ERROR_CONVERTBASE_ALLOCATION = 4,
    ERROR_CONVERTBASE_INVALID_REPRESENTATION = 5
} error_convertbase;

/**
* convert from base b to decimal
* @param string base
* @param int base
* @param[out] int if 0 no error
* @return converted value (decimal)
*/
double convertBase2Dec(const char *pStr, int numberbase, error_convertbase *err);

/**
* Convert decimal to base N number in string
* @param[in] array of integer values
* @param[in] number of elements in dValues array
* @param[in] number of digits for representation
* @param[out] error value
* @return a matrix of string of size mn
*/
char **convertMatrixOfDec2Base(const double* dValues, int mn, int numberbase, unsigned int nbDigits, error_convertbase *err);

#endif /* __CONVERTBASE_H__ */
/*--------------------------------------------------------------------------*/
