/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* CORNET Allan */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__
/*-----------------------------------------------------------------------------------*/ 
//#include "../../libs/hashtable/hashtable.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
struct key_string
{
	char *Key_String;
};
/*-----------------------------------------------------------------------------------*/ 
struct value_string
{
	char *Value_String;
};
/*-----------------------------------------------------------------------------------*/ 
/* new a hashtable and return it */
struct hashtable *CreateHashtable_string();

/* destroy the hashtable and clean the resources used */
void DestroyHashtable_string(struct hashtable *hash_table);

/* search a key in a hashtable. if succeed, return the value needed. if fail, return NULL */
char *SearchHashtable_string(struct hashtable *hash_table, const char* key);

/* Insert a element in Hashtable */
int InsertHashtable_string(struct hashtable *hash_table,struct key_string *k, struct value_string *v);

/*-----------------------------------------------------------------------------------*/ 
#endif /* __HASHTABLE_H__ */
/*-----------------------------------------------------------------------------------*/ 
