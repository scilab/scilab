/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* HUANG Xu */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __LOCALIZATION_H__
#define __LOCALIZATION_H__
/*-----------------------------------------------------------------------------------*/ 
#include "xmlreader.h"
#include <string.h>
#include <iconv.h>
#include "hashtable_localization.h"
/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
	#ifdef EXPORT_LOCALIZATION_DLL
		#define IMPORT_EXPORT_LOCALIZATION_DLL __declspec(dllexport)
	#else
		#define IMPORT_EXPORT_LOCALIZATION_DLL __declspec(dllimport)
	#endif
#else
	#define IMPORT_EXPORT_LOCALIZATION_DLL extern 
#endif
/*-----------------------------------------------------------------------------------*/ 
/*load the xmlfile into the hashtable*/
IMPORT_EXPORT_LOCALIZATION_DLL int AppendXmlFile(const char *filename, struct hashtable *table);
/*return pointer on HashTable Scilab Errors*/
IMPORT_EXPORT_LOCALIZATION_DLL struct hashtable *GetHashTableScilabErrors(void);
IMPORT_EXPORT_LOCALIZATION_DLL int InitializeHashTableScilabErrors(char* SCIPATH);
IMPORT_EXPORT_LOCALIZATION_DLL char *QueryStringError(char *Tag);
/*-----------------------------------------------------------------------------------*/ 
#endif /* __LOCALIZATION_H__ */
/*-----------------------------------------------------------------------------------*/ 
