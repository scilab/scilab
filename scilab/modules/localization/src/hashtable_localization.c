/*-----------------------------------------------------------------------------------*/ 
/* CORNET Allan */
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#include "hashtable_localization.h"
#include <string.h>
/*-----------------------------------------------------------------------------------*/ 
/* see http://www.cse.yorku.ca/~oz/hash.html */
/* I choose djb2 algo. for strings */
static unsigned int hashfromkey_string(void *ky)
{
	struct key_string *k = NULL;
	unsigned char *str = NULL;

	unsigned long hash = 5381;
	int c;
		
	k =	(struct key_string *)ky;
	str=k->Key_String;

	while (c = *str++) 	hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;

}
/*-----------------------------------------------------------------------------------*/ 
static int equalkeys_string(void *k1, void *k2)
{
	int bequal=0;
	struct key_string *K1 = (struct key_string *)k1;
	struct key_string *K2 = (struct key_string *)k2;

	if (strcmp(K1->Key_String,K2->Key_String)==0) bequal=1;
	else bequal=0;
	
	return bequal;
}
/*-----------------------------------------------------------------------------------*/ 
struct hashtable *CreateHashtable_string()
{
	return create_hashtable(16, hashfromkey_string, equalkeys_string);
}
/*-----------------------------------------------------------------------------------*/ 
void DestroyHashtable_string(struct hashtable *hash_table)
{
	hashtable_destroy(hash_table, 1);
}
/*-----------------------------------------------------------------------------------*/ 
char *SearchHashtable_string(struct hashtable *hash_table, const char* key)
{
	int i=0;
	struct key_string *k;
	struct key_string *kElem=NULL;
	

	k=(struct key_string*)MALLOC(sizeof(struct key_string));
	k->Key_String=MALLOC((strlen(key)+1)*sizeof(char));
	strcpy(k->Key_String,key);
	kElem=hashtable_search(hash_table,k);

	FREE(k);
	return kElem->Key_String;
}
/*-----------------------------------------------------------------------------------*/ 
int InsertHashtable_string(struct hashtable *hash_table,struct key_string *k, struct value_string *v)
{
	return hashtable_insert(hash_table,k,v);
}
/*-----------------------------------------------------------------------------------*/ 
