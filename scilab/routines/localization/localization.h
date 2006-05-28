/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* HUANG Xu */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __LOCALIZATION_H__
#define __LOCALIZATION_H__
/*-----------------------------------------------------------------------------------*/ 
#include <libxml/xmlreader.h>
#include <glib.h>
#include <string.h>
#include <iconv.h>
#include "../hashtable/hashtable.h"
/*-----------------------------------------------------------------------------------*/ 
/*load the xmlfile into the hashtable*/
int AppendXmlFile(const char *filename, GHashTable *table, char* encoding) ;

/*return pointer on HashTable Scilab Errors*/
GHashTable *GetHashTableScilabErrors(void);
int InitializeHashTableScilabErrors(char* SCIPATH);
char *QueryStringError(char *Tag);
/*-----------------------------------------------------------------------------------*/ 
#endif /* __LOCALIZATION_H__ */
/*-----------------------------------------------------------------------------------*/ 