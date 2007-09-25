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
BOOL AppendHashTableLocalization(struct hashtable *Table,char *Tag,char* MsgStr,char* Path);
/*-----------------------------------------------------------------------------------*/ 
#endif /* __LOCALIZATION_H__ */
/*-----------------------------------------------------------------------------------*/ 
