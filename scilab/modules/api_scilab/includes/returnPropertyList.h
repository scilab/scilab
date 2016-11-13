/*
 * Scilab (http://www.scilab.org/) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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

#ifndef _RETURN_PROPERTY_LIST_H_
#define _RETURN_PROPERTY_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "dynlib_api_scilab.h"

/**
 * Construct a new tlist for return in the console.
 * Only one tlist should be used at the same time.
 * @param nbElement Number of element of element in the list not including the name.
 * @param name      Array of string of size nbElement containing the names of the list
 *                  and its properties. Its size is nbElement + 1.
 * @return a pointer to the newly created list or NULL if an error occurred.
 */
void* createReturnedList(int nbElements, char* elementName[]);

/**
 * add a new row vector in the list.
 * @param[in/out] void the returned tList
 * @param[in]     vector       values to be inserted of size nbValues
 * @return        0  if the call was successful
 */
int addRowVectorToReturnedList(void* list, const double vector[], int nbValues);

/**
 * add a new column vector in the list.
 * @param[in/out] void the returned tList
 * @param[in]     vector       values to be inserted of size nbValues
 * @return        0  if the call was successful
 */
int addColVectorToReturnedList(void* list, const double vector[], int nbValues);

/**
 * add a new column vector in the list.
 * @param[in/out] void the returned tList
 * @param[in]     matrix   values to be inserted of size nbRow x nbCol
 * @return        0  if the call was successful
 */
int addMatrixToReturnedList(void* list, const double matrix[], int nbRow, int nbCol);

/**
 * add a new string column vector in the list.
 * @param[in/out] void the returned tList
 * @param[in]     vector   values to be inserted of size nbValues
 * @return        0  if the call was successful
 */
int addStringColVectorToReturnedList(void* list, char* vector[], int nbValues);
int addStringToReturnedList(void* list, char * str);
#ifdef __cplusplus
}
#endif
#endif /* _RETURN_PROPERTY_LIST_H_ */
