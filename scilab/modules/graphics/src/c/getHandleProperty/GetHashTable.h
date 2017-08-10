/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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

/*------------------------------------------------------------------------*/
/* file: GetHashTable.h                                                   */
/* desc : implementation of the scilab hashtable for the get procedure    */
/*------------------------------------------------------------------------*/

#ifndef _GET_HASH_TABLE_H_
#define _GET_HASH_TABLE_H_

#include "setGetHashTable.h"

#include "dynlib_graphics.h"
/**
 * create the fill hash table already filled for get functions
 * this use the singleton pattern since only one hashtable is useful.
 */
GetPropertyHashTable * createScilabGetHashTable(void);

/**
 * Delete the scilab get hashtable
 */
void destroyScilabGetHashTable(void);

/**
 * call the function which the property propertyName of object pObj
 * @return  0 if successful
 *         -1 if an error occurred in the get function
 *          1 if the property was not found
 */
GRAPHICS_IMPEXP void* callGetProperty(void* _pvCtx, int iObjUID, char * propertyName);

#endif /* _GET_HASH_TABLE_H_ */
