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

#include <string.h>
#include "hashtable_localization.h"
/*--------------------------------------------------------------------------*/ 
/* see http://www.cse.yorku.ca/~oz/hash.html */
/* I choose djb2 algo. for strings */
static unsigned int hashfromkey_string(void *ky)
{
	struct key_string *k = NULL;
	unsigned char *str = NULL;

	unsigned long hash = 5381;
	int c;
		
	k =	(struct key_string *)ky;
	str=(unsigned char*)k->Key_String;

	while ((c = *str++)) hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;

}
/*--------------------------------------------------------------------------*/ 
static int equalkeys_string(void *k1, void *k2)
{
	int bequal=0;
	struct key_string *K1 = (struct key_string *)k1;
	struct key_string *K2 = (struct key_string *)k2;

	if (strcmp(K1->Key_String,K2->Key_String)==0) bequal=1;
	else bequal=0;
	
	return bequal;
}
/*--------------------------------------------------------------------------*/ 
struct hashtable *CreateHashtable_string()
{
	return create_hashtable(16, hashfromkey_string, equalkeys_string);
}
/*--------------------------------------------------------------------------*/ 
void DestroyHashtable_string(struct hashtable *hash_table)
{
	hashtable_destroy(hash_table, 1);
}
/*--------------------------------------------------------------------------*/ 
char *SearchHashtable_string(struct hashtable *hash_table, const char* key)
/* return a copy of the string in hashtable */
{
	char *FindString=NULL;
	struct key_string *k;
	struct value_string *kElem=NULL;

	k=(struct key_string*)MALLOC(sizeof(struct key_string));
	k->Key_String=MALLOC((strlen(key)+1)*sizeof(char));
	strcpy(k->Key_String,key);

	kElem=hashtable_search(hash_table,k);

	FREE(k);

	if (kElem) 
	{
		FindString=(char*)MALLOC(sizeof(char)*(strlen(kElem->Value_String)+1));
		if (FindString) strcpy(FindString,kElem->Value_String);
	}

	return FindString;
}
/*--------------------------------------------------------------------------*/ 
int InsertHashtable_string(struct hashtable *hash_table,struct key_string *k, struct value_string *v)
{
 	return hashtable_insert(hash_table,k,v);
}
/*--------------------------------------------------------------------------*/ 
BOOL RemoveHastable_string(struct hashtable *hash_table, const char* key)
{
	BOOL bOK=FALSE;
	struct value_string *kElem=NULL;
	struct key_string *k;

	k=(struct key_string*)MALLOC(sizeof(struct key_string));
	k->Key_String=MALLOC((strlen(key)+1)*sizeof(char));
	strcpy(k->Key_String,key);

	kElem=hashtable_remove(hash_table,k);
	FREE(k);

	if (kElem)
	{
		bOK=TRUE;
		FREE(kElem);
	}
	else
	{
		bOK=FALSE;
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
