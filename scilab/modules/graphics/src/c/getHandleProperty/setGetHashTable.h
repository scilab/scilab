/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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
/* file: setGetHashTable.h                                                */
/* desc : define two hash table to be used in sci_set and sci_get         */
/*        These hash table are based on the Scilab hashTable              */
/*------------------------------------------------------------------------*/

#ifndef _SET_GET_HASHTABLE_H_
#define _SET_GET_HASHTABLE_H_

#include <stdlib.h>

#include "hashtable.h"

/*--------------------------------------------------------------------------*/
/**
 * Hide the real type of hashtable
 */
typedef struct hashtable GetPropertyHashTable;

/**
* Hide the real type of hashtable
*/
typedef struct hashtable SetPropertyHashTable;
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/**
 * Prototype of functions used to get a specific property of an handle.
 * The char* is the UID of the object to get the property from
 * The return value is 0 if the call was successful and -1 otherwise.
 */
typedef void* (*getPropertyFunc)(void*, int);

/**
* Prototype of functions used to set a specific property of an handle.
* The char* is the UID of the object to get the property from
* The return value is SET_PROPERTY_SUCCEED if the call was successful and a redraw is needed
* SET_PROPERTY_UNCHANGED if nothing was actually changed and SET_PROPERTY_ERROR if
* an error occurred.
*/
typedef int (*setPropertyFunc)(void*, int, void*, int, int, int);
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/**
 * Create a new hashTable of get functions
 */
GetPropertyHashTable * createGetHashTable(void);

/**
 * destroy the hashtable and free the used resources.
 */
void destroyGetHashTable(GetPropertyHashTable * hashTable);

/**
 * Search for a key in a hashtable. if succeed, return the value needed. if failed, return NULL
 * @return The corresponding value if the key was found, NULL otherwise.
 */
getPropertyFunc searchGetHashtable(GetPropertyHashTable * hashTable , char * key);

/*
 * Insert a new element in the hashtable
 */
int insertGetHashtable(GetPropertyHashTable * hashTable, char * key, getPropertyFunc value);

/*--------------------------------------------------------------------------*/
/**
* Create a new hashTable of set functions
*/
GetPropertyHashTable * createSetHashTable(void);

/**
* destroy the hashtable and free the used resources.
*/
void destroySetHashTable(SetPropertyHashTable * hashTable);

/**
* Search for a key in a hashtable. if succeed, return the value needed. if failed, return NULL
* @return The corresponding value if the key was found, NULL otherwise.
*/
setPropertyFunc searchSetHashtable(SetPropertyHashTable * hashTable , char * key);

/*
* Insert a new element in the hashtable
*/
int insertSetHashtable(SetPropertyHashTable * hashTable, char * key, setPropertyFunc value);

/*--------------------------------------------------------------------------*/

#endif /* _SET_GET_HASHTABLE_H_ */
