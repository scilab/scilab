/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* HUANG Xu */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include <string.h>
#include "localization.h"
#include "MALLOC.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/ 
static struct hashtable *Table_Scilab_Errors=NULL;
static struct hashtable *Table_Scilab_Messages=NULL;
static struct hashtable *Table_Scilab_Menus=NULL;
/*-----------------------------------------------------------------------------------*/ 
static char *QueryString(struct hashtable *Table,char *Tag);
/*-----------------------------------------------------------------------------------*/ 
BOOL AppendHashTableLocalization(struct hashtable *Table,char *Tag,char* MsgStr,char *Path)
{
	BOOL bOK=FALSE;
	struct key_string *k=NULL;
	struct value_string *v=NULL;
	char *Key_Tag=NULL;
	char *Key_Value=NULL;
	char *Key_Path = NULL;

	k=(struct key_string*)MALLOC(sizeof(struct key_string));
	v=(struct value_string*)MALLOC(sizeof(struct value_string));

	Key_Tag=(char*)MALLOC(sizeof(char)*(strlen(Tag)+1));
	Key_Value=(char*)MALLOC(sizeof(char)*(strlen(MsgStr)+1));
	Key_Path=(char*)MALLOC(sizeof(char)*(strlen(Path)+1));

	if (k && v && Key_Tag && Key_Value && Key_Path)
	{
		strcpy(Key_Tag,Tag);
		strcpy(Key_Value,MsgStr);
		strcpy(Key_Path,Path);

		k->Key_String = Key_Tag;
		v->Value_String = Key_Value;
		v->Path_String = Key_Path;

		if (InsertHashtable_string(Table,k, v)) bOK=TRUE;

	}
	return bOK;

}
/*-----------------------------------------------------------------------------------*/
struct hashtable *GetHashTableScilabErrors(void)
{
	return Table_Scilab_Errors;
}
/*-----------------------------------------------------------------------------------*/ 
struct hashtable *GetHashTableScilabMessages(void)
{
	return Table_Scilab_Messages;
}
/*-----------------------------------------------------------------------------------*/ 
struct hashtable *GetHashTableScilabMenus(void)
{
	return Table_Scilab_Menus;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeHashTableScilabErrors(void)
{
	Table_Scilab_Errors=CreateHashtable_string();
	if (Table_Scilab_Errors) return TRUE;
	else return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeHashTableScilabMessages(void)
{
	Table_Scilab_Messages=CreateHashtable_string();
	if (Table_Scilab_Messages) return TRUE;
	else return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeHashTableScilabMenus(void)
{
	Table_Scilab_Menus=CreateHashtable_string();
	if (Table_Scilab_Menus) return TRUE;
	else return FALSE;
}
/*-----------------------------------------------------------------------------------*/ 
int DisposeHashTableScilabErrors(void)
{
	if (Table_Scilab_Errors)
	{
		hashtable_destroy( Table_Scilab_Errors, 0 ) ;
		Table_Scilab_Errors=NULL;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
int DisposeHashTableScilabMessages(void)
{
	if (Table_Scilab_Messages)
	{
		hashtable_destroy( Table_Scilab_Messages, 0 ) ;
		Table_Scilab_Messages=NULL;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
int DisposeHashTableScilabMenus(void)
{
	if (Table_Scilab_Menus)
	{
		hashtable_destroy(Table_Scilab_Menus, 0 ) ;
		Table_Scilab_Menus=NULL;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/ 
char *QueryStringError(char *Tag)
{	
	return QueryString(Table_Scilab_Errors,Tag);
}
/*-----------------------------------------------------------------------------------*/ 
char *QueryStringMessage(char *Tag)
{
	return QueryString(Table_Scilab_Messages,Tag);
}
/*-----------------------------------------------------------------------------------*/ 
char *QueryStringMenu(char *Tag)
{
	return QueryString(Table_Scilab_Menus,Tag);
}
/*-----------------------------------------------------------------------------------*/ 
static char *QueryString(struct hashtable *Table,char *Tag)
{
	char *RetString=NULL;
	if (Table==NULL)
	{
		printf("Internal error: localization table empty (looking for %s)!\nAre you sure that the localization files have been loaded ?\n",Tag);
		exit(-1);
	}
	RetString = SearchHashtable_string(Table,Tag);
	return RetString;
}
/*-----------------------------------------------------------------------------------*/ 
