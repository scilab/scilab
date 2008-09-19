
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdlib.h>
#include "MALLOC.h"
#include "hashtable_metanet.h"
/*--------------------------------------------------------------------------*/
static struct hashtable *Table_Metanet=NULL;
static unsigned hsize;
static unsigned filled;
/*--------------------------------------------------------------------------*/
struct key_string
{
	char *Key_String;
};
/*--------------------------------------------------------------------------*/
struct value_string
{
	char *Value_String;
};
/*--------------------------------------------------------------------------*/
static unsigned int hashfromkey_string(void *ky);
static int equalkeys_string(void *k1, void *k2);
static int isprime(unsigned number);
static int InsertHashtable_string(struct hashtable *hash_table,struct key_string *k, struct value_string *v);
static ENTRY *SearchHashtable_string(struct hashtable *hash_table, const char* key);
/*--------------------------------------------------------------------------*/
/* see http://www.cse.yorku.ca/~oz/hash.html */
/* I choose djb2 algo. for strings */
static unsigned int hashfromkey_string(void *ky)
{
	struct key_string *k = NULL;
	char *str = NULL;

	unsigned long hash = 5381;
	int c;

	k =	(struct key_string *)ky;
	str=k->Key_String;

	while ((c = *(str++)))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

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
static int isprime(unsigned number)
{
	/* no even number will be passed */
	unsigned div_ = 3;

	while (div_*div_ < number && number%div_ != 0)
		div_ += 2;

	return number%div_ != 0;
}
/*--------------------------------------------------------------------------*/
int myhcreate(unsigned nel)
{
	int bOK=0;
	if (Table_Metanet == NULL)
	{
		nel |= 1;      /* make odd */
		while (!isprime(nel)) nel += 2;

		hsize  = nel;
		filled = 0;

		Table_Metanet =create_hashtable(nel, hashfromkey_string, equalkeys_string);
		if (Table_Metanet) bOK=1;
	}

	return bOK;
}
/*--------------------------------------------------------------------------*/
void myhdestroy()
{
	if (Table_Metanet)
	{
		hashtable_destroy(Table_Metanet, 1);
		Table_Metanet = NULL;
	}
}
/*--------------------------------------------------------------------------*/
static int InsertHashtable_string(struct hashtable *hash_table,struct key_string *k, struct value_string *v)
{
	return hashtable_insert(hash_table,k,v);
}
/*--------------------------------------------------------------------------*/
/* search a key in a hashtable. if succeed, return the ENTRY needed. if fail, return NULL */
static ENTRY *SearchHashtable_string(struct hashtable *hash_table, const char* key)
{
	ENTRY* return_entry=NULL;
	struct key_string *k=NULL;

	k=(struct key_string*)MALLOC(sizeof(struct key_string));

	if (k)
	{
		struct key_string *kElem=NULL;

		k->Key_String=MALLOC((strlen(key)+1)*sizeof(char));
		strcpy(k->Key_String,key);

		kElem=hashtable_search(hash_table,k);

		if (kElem)
		{
			return_entry=(ENTRY*)MALLOC(sizeof(ENTRY));
			return_entry->key=(char*)MALLOC(sizeof(char)*(strlen(key)+1));
			return_entry->data=(char*)MALLOC(sizeof(char)*(strlen(kElem->Key_String)+1));

			strcpy(return_entry->key,key);
			strcpy(return_entry->data,kElem->Key_String);
		}

		FREE(k->Key_String);
		k->Key_String=NULL;
		FREE(k);
		k=NULL;
	}

	return return_entry;
}
/*--------------------------------------------------------------------------*/
ENTRY* myhsearch(ENTRY item,SCIACTION action)
{
	ENTRY* return_entry=NULL;

	if (action == SCIENTER)
	{
		if (filled == hsize)
		{
			/*
			* If table is full and another entry should be entered return with
			* error.
			*/
			return_entry=NULL;
		}
		else
		{
			/* add a element */
			struct key_string *k=NULL;
			struct value_string *v=NULL;

			k = (struct key_string*)MALLOC(sizeof(struct key_string));
			v = (struct value_string*)MALLOC(sizeof(struct value_string));

			k->Key_String = (char *) MALLOC( sizeof(char) * (strlen(item.key)+1) );
			v->Value_String = (char *) MALLOC( sizeof(char) * (strlen(item.data)+1) );

			strcpy(k->Key_String,item.key);
			strcpy(v->Value_String,item.data);

			InsertHashtable_string(Table_Metanet,k,v);
			filled++;

			return_entry=SearchHashtable_string(Table_Metanet, item.key);
		}
	}
	else if (action == SCIFIND)
	{
		return_entry=SearchHashtable_string(Table_Metanet, item.key);
	}

	return return_entry;
}
/*--------------------------------------------------------------------------*/
