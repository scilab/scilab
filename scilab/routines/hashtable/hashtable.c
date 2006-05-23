/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* HUANG Xu */
/*-----------------------------------------------------------------------------------*/ 
#include "hashtable.h"
/*-----------------------------------------------------------------------------------*/ 
void key_destroy_func(gpointer key)
{
	g_free(key);
}
/*-----------------------------------------------------------------------------------*/ 
void value_destroy_func(gpointer value)
{
	g_free(value);
}
/*-----------------------------------------------------------------------------------*/ 
GHashTable * CreateHashtable()
{
	return g_hash_table_new_full(g_str_hash, g_str_equal, (GDestroyNotify )key_destroy_func, (GDestroyNotify )value_destroy_func);
}
/*-----------------------------------------------------------------------------------*/ 
void free_a_hash_table_entry(gpointer gkey, gpointer gvalue, gpointer user_data)
{
	g_free(gkey);
	g_free(gvalue);
	
}
/*-----------------------------------------------------------------------------------*/ 
void DestroyHashtable(GHashTable *table)
{
	g_hash_table_foreach(table,(GHFunc)free_a_hash_table_entry,NULL);
	g_hash_table_destroy(table);
}
/*-----------------------------------------------------------------------------------*/ 
char *SearchHash(GHashTable *hash_table, const char* key)
{
	return (char *)g_hash_table_lookup(hash_table, key);
}
/*-----------------------------------------------------------------------------------*/ 

