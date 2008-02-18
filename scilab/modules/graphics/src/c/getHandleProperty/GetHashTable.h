/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: GetHashTable.h                                                   */
/* desc : implementation of the scilab hashtable for the get procedure    */
/*------------------------------------------------------------------------*/

#ifndef _GET_HASH_TABLE_H_
#define _GET_HASH_TABLE_H_

#include "setGetHashTable.h"
#include "ObjectStructure.h"

/**
 * create the fill hash table already filled for get functions
 * this use the singleton pattern since only one hashtable is useful.
 */
GetPropertyHashTable * createScilabGetHashTable( void ) ;

/**
 * Delete the scilab get hashtable
 */
void destroyScilabGetHashTable( void ) ;

/**
 * call the function which the property propertyName of object pObj
 * @return  0 if successful
 *         -1 if an error occured in the get function
 *          1 if the property was not found
 */
int callGetProperty( sciPointObj * pObj, char * propertyName ) ;

#endif /* _GET_HASH_TABLE_H_ */
