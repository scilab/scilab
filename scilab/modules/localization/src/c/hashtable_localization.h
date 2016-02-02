/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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

#ifndef __HASHTABLE_LOCALIZATION_H__
#define __HASHTABLE_LOCALIZATION_H__
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include "hashtable.h"
#include "BOOL.h" /* BOOL */
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
struct key_string
{
    char *Key_String;
};
/*--------------------------------------------------------------------------*/
struct value_string
{
    char *Value_String;
    char *Path_String;
};
/*--------------------------------------------------------------------------*/
/**
 * create a hashtable and return it
 */
struct hashtable *CreateHashtable_string(void);

/* destroy the hashtable and clean the resources used */
void DestroyHashtable_string(struct hashtable *hash_table);

/* search a key in a hashtable. if succeed, return the value needed. if failed, return NULL */
char *SearchHashtable_string(struct hashtable *hash_table, const char* key);

/* Insert a element in Hashtable */
int InsertHashtable_string(struct hashtable *hash_table, struct key_string *k, struct value_string *v);

/* Remove a element in Hastable */
BOOL RemoveHastable_string(struct hashtable *hash_table, const char* key);
/*--------------------------------------------------------------------------*/
#endif /* __HASHTABLE_LOCALIZATION_H__ */
/*--------------------------------------------------------------------------*/
