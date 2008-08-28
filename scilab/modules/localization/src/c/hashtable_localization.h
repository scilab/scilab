/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __HASHTABLE_LOCALIZATION_H__
#define __HASHTABLE_LOCALIZATION_H__
/*--------------------------------------------------------------------------*/ 
#include <stdlib.h>
#include "hashtable.h"
#include "machine.h" /* BOOL */
#include "MALLOC.h"
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
int InsertHashtable_string(struct hashtable *hash_table,struct key_string *k, struct value_string *v);

/* Remove a element in Hastable */
BOOL RemoveHastable_string(struct hashtable *hash_table, const char* key);
/*--------------------------------------------------------------------------*/ 
#endif /* __HASHTABLE_LOCALIZATION_H__ */
/*--------------------------------------------------------------------------*/ 
