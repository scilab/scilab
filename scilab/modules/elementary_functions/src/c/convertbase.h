/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Allan CORNET
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
#ifndef __CONVERTBASE_H__
#define __CONVERTBASE_H__

#include "dynlib_elementary_functions.h"

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
ELEMENTARY_FUNCTIONS_IMPEXP double convertBase2Dec(const char *pStr, int numberbase, error_convertbase *err);

/**
* Convert decimal to base N number in string
* @param[in] array of integer values
* @param[in] number of elements in dValues array
* @param[in] number of digits for representation
* @param[out] error value
* @return a matrix of string of size mn
*/
ELEMENTARY_FUNCTIONS_IMPEXP char **convertMatrixOfDec2Base(const double* dValues, int mn, int numberbase, unsigned int nbDigits, error_convertbase *err);

#endif /* __CONVERTBASE_H__ */
/*--------------------------------------------------------------------------*/
