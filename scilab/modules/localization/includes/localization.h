/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* HUANG Xu */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __LOCALIZATION_H__
#define __LOCALIZATION_H__
/*-----------------------------------------------------------------------------------*/ 
#include "hashtable_localization.h"
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
	#ifdef EXPORT_LOCALIZATION_DLL
		#define IMPORT_EXPORT_LOCALIZATION_DLL __declspec(dllexport)
	#else
		#define IMPORT_EXPORT_LOCALIZATION_DLL __declspec(dllimport)
	#endif
#else
	#define IMPORT_EXPORT_LOCALIZATION_DLL extern 
#endif
/*-----------------------------------------------------------------------------------*/ 
IMPORT_EXPORT_LOCALIZATION_DLL char *QueryStringError(char *Tag);
IMPORT_EXPORT_LOCALIZATION_DLL char *QueryStringMessage(char *Tag);
IMPORT_EXPORT_LOCALIZATION_DLL char *QueryStringMenu(char *Tag);
/*-----------------------------------------------------------------------------------*/ 
struct hashtable *GetHashTableScilabErrors(void);
struct hashtable *GetHashTableScilabMessages(void);
struct hashtable *GetHashTableScilabMenus(void);
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeHashTableScilabErrors(void);
BOOL InitializeHashTableScilabMessages(void);
BOOL InitializeHashTableScilabMenus(void);
/*-----------------------------------------------------------------------------------*/ 
int DisposeHashTableScilabErrors(void);
int DisposeHashTableScilabMessages(void);
int DisposeHashTableScilabMenus(void);
/*-----------------------------------------------------------------------------------*/ 
BOOL AppendHashTableLocalization(struct hashtable *Table,char *Tag,char* MsgStr);
/*-----------------------------------------------------------------------------------*/ 
#endif /* __LOCALIZATION_H__ */
/*-----------------------------------------------------------------------------------*/ 
