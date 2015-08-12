/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2007 - INRIA - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: setGetHashTable.h                                                */
/* desc : define two hash table to be used in sci_set and sci_get         */
/*        These hash table are based on the Scilab hashTable              */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <ctype.h>
#include "stricmp.h"

#include "setGetHashTable.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
/* see http://www.cse.yorku.ca/~oz/hash.html */
/* like in hashtable_localization by Allan Cornet */
/* I choose djb2 algo. for strings */
static unsigned int setGetHashTableHash(void * key)
{
    unsigned long hash = 5381;
    int c = 0;
    char * str = key;

    while ((c = tolower(*str++)))
    {
        hash = ((hash << 5) + hash) + c ; /* hash * 33 + c */
    }

    return hash;

}
/*--------------------------------------------------------------------------*/
static int setGetHashTableEqualKeys(void * k1, void * k2)
{
    if (stricmp((char *)k1, (char *)k2) == 0)
    {
        return 1;
    }
    return 0;
}
/*--------------------------------------------------------------------------*/
GetPropertyHashTable * createGetHashTable(void)
{
    return create_hashtable(16, setGetHashTableHash, setGetHashTableEqualKeys);
}
/*--------------------------------------------------------------------------*/
void destroyGetHashTable(GetPropertyHashTable * hashTable)
{
    /* we just store pointers */
    hashtable_destroy(hashTable, 0);
}
/*--------------------------------------------------------------------------*/
getPropertyFunc searchGetHashtable(GetPropertyHashTable * hashTable, char * key)
{
    return (getPropertyFunc) hashtable_search(hashTable, key);
}
/*--------------------------------------------------------------------------*/
int insertGetHashtable(GetPropertyHashTable * hashTable, char * key, getPropertyFunc value)
{
    /* allocate a new key because the hashtable claims ownership */
    /* and will free it when destroyed */
    char * copyKey  =  strdup(key);
    if (copyKey == NULL)
    {
        return 0 ;
    }
    return hashtable_insert(hashTable, copyKey, value);
}
/*--------------------------------------------------------------------------*/
SetPropertyHashTable * createSetHashTable(void)
{
    return create_hashtable(16, setGetHashTableHash, setGetHashTableEqualKeys);
}
/*--------------------------------------------------------------------------*/
void destroySetHashTable(SetPropertyHashTable * hashTable)
{
    /* we just store pointers */
    hashtable_destroy(hashTable, 0);
}
/*--------------------------------------------------------------------------*/
setPropertyFunc searchSetHashtable(SetPropertyHashTable * hashTable, char * key)
{
    return (setPropertyFunc) hashtable_search(hashTable, key);
}
/*--------------------------------------------------------------------------*/
int insertSetHashtable(SetPropertyHashTable * hashTable, char * key, setPropertyFunc value)
{
    /* allocate a new key because the hashtable claims ownership */
    /* and will free it when destroyed */
    char * copyKey   = strdup(key);
    if (copyKey == NULL)
    {
        return 0 ;
    }
    return hashtable_insert(hashTable, copyKey, value);
}
/*--------------------------------------------------------------------------*/
