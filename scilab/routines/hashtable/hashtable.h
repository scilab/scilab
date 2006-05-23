/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* HUANG Xu */
/* CORNET Allan */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__
/*-----------------------------------------------------------------------------------*/ 
#include <glib.h>
/*-----------------------------------------------------------------------------------*/ 
//new a hashtable and return it
GHashTable *CreateHashtable();

//search a key in a hashtable. if succeed, return the value needed. if fail, return NULL
char *SearchHash(GHashTable *hash_table, const char* key);

//destroy the hashtable and clean the resources used
void DestroyHashtable(GHashTable *table);

void key_destroy_func(gpointer key);

void value_destroy_func(gpointer value);

void free_a_hash_table_entry(gpointer gkey, gpointer gvalue, gpointer user_data);

/*-----------------------------------------------------------------------------------*/ 
#endif /* __HASHTABLE_H__ */
/*-----------------------------------------------------------------------------------*/ 