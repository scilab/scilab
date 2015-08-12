/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */


#ifndef _RETURN_PROPERTY_LIST_H_
#define _RETURN_PROPERTY_LIST_H_

typedef struct
{
    int curElement ; /**< the number of the current element in the stack the first one is the name.
                        vary from 1 to nbElements */
    int nbElements ; /**< the number of element which will be inserted in the list. */
    int stackPointer ; /**< use to differentiate returned list. Not useful for now. */
} returnedList ;

/**
 * Construct a new tlist and allocate scilab stack for return in the console.
 * Only one tlist should be used at the same time.
 * @param nbElement Number of element of element in the list not including the name.
 * @param name      Array of string of size nbElement containing the names of the list
 *                  and its properties. Its size is nbElement + 1.
 * @return a pointer to the newly created list or NULL if an error occurred.
 */
returnedList * createReturnedList( int nbElements, char * elementName[] ) ;

/**
 * free the list. Must be done after using the list.
 * @return  0 if deletion was successful
 *          1 if the list has not been totally filled
 *         -1 if an other error happened
 */
int destroyReturnedList( returnedList * list ) ;

/**
 * add a new row vector in the list.
 * @param[in/out] returnedList the returned tList
 * @param[in]     vector       values to be inserted of size nbValues
 * @return        0  if the call was successful
 *                1  if the list is full so the item can not be inserted
 *                -1 if an other error occurred
 */
int addRowVectorToReturnedList( returnedList * list, const double vector[], int nbValues ) ;

/**
 * add a new column vector in the list.
 * @param[in/out] returnedList the returned tList
 * @param[in]     vector       values to be inserted of size nbValues
 * @return        0  if the call was successful
 *                1  if the list is full so the item can not be inserted
 *                -1 if an other error occurred
 */
int addColVectorToReturnedList( returnedList * list, const double vector[], int nbValues ) ;

/**
 * add a new column vector in the list.
 * @param[in/out] returnedList the returned tList
 * @param[in]     matrix   values to be inserted of size nbRow x nbCol
 * @return        0  if the call was successful
 *                1  if the list is full so the item can not be inserted
 *                -1 if an other error occurred
 */
int addMatrixToReturnedList( returnedList * list, const double matrix[], int nbRow, int nbCol ) ;

/**
 * add a new string column vector in the list.
 * @param[in/out] returnedList the returned tList
 * @param[in]     vector   values to be inserted of size nbValues
 * @return        0  if the call was successful
 *                1  if the list is full so the item can not be inserted
 *                -1 if an other error occurred
 */
int addStringColVectorToReturnedList( returnedList * list, char * vector[], int nbValues ) ;
int addStringToReturnedList( returnedList * list, char * str) ;

#endif /* _RETURN_PROPERTY_LIST_H_ */
