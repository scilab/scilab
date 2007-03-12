/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* HUANG Xu */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include <string.h>
#include "localization.h"
#include "QueryStringError.h"
#include "QueryStringMessage.h"
#include "QueryStringMenu.h"
#include "MALLOC.h" /* MALLOC */
/*-----------------------------------------------------------------------------------*/ 
static int count=0;//count the number of the #text and type==3 node 
static char *Key_String=NULL;

static struct hashtable *Table_Scilab_Errors=NULL;
static struct hashtable *Table_Scilab_Messages=NULL;
static struct hashtable *Table_Scilab_Menus=NULL;

static char *ReplaceChars(char *s1, char *s2, char *s3);
static char *QueryString(struct hashtable *Table,char *Tag);
/*-----------------------------------------------------------------------------------*/ 
BOOL AppendHashTableLocalization(struct hashtable *Table,char *Tag,char* MsgStr)
{
	BOOL bOK=FALSE;
	struct key_string *k=NULL;
	struct value_string *v=NULL;
	char *Key_Tag=NULL;
	char *Key_Value=NULL;

	k=(struct key_string*)MALLOC(sizeof(struct key_string));
	v=(struct value_string*)MALLOC(sizeof(struct value_string));

	Key_Tag=(char*)MALLOC(sizeof(char)*(strlen(Tag)+1));
	Key_Value=(char*)MALLOC(sizeof(char)*(strlen(MsgStr)+1));

	if (k && v && Key_Tag && Key_Value)
	{
		strcpy(Key_Tag,Tag);
		strcpy(Key_Value,MsgStr);

		k->Key_String=Key_Tag;
		v->Value_String=Key_Value;

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
	BOOL bOK=FALSE;

	Table_Scilab_Errors=CreateHashtable_string();
	if (Table_Scilab_Errors) bOK=TRUE;
	else bOK=FALSE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeHashTableScilabMessages(void)
{
	BOOL bOK=FALSE;

	Table_Scilab_Messages=CreateHashtable_string();
	if (Table_Scilab_Messages) bOK=TRUE;
	else bOK=FALSE;

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL InitializeHashTableScilabMenus(void)
{
	BOOL bOK=FALSE;

	Table_Scilab_Menus=CreateHashtable_string();
	if (Table_Scilab_Menus) bOK=TRUE;
	else bOK=FALSE;

	return bOK;
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
static char *ReplaceChars(char *S1, char *S2, char *S3) 
{
	char *buffer=NULL;

	if ( (S1) && (strlen(S1)>0) )
	{
		char *p=NULL;
		buffer = (char*)MALLOC((strlen(S1)*2)*sizeof(char));
		if(!(p = strstr(S1, S2))) 
		{
			sprintf(buffer,"%s",S1);
			return buffer;
		}

		strncpy(buffer, S1, p-S1);
		buffer[p-S1] = '\0';

		sprintf(buffer+(p-S1), "%s%s", S3, p+strlen(S2));
	}
	return buffer;
}
/*-----------------------------------------------------------------------------------*/ 
static char *QueryString(struct hashtable *Table,char *Tag)
{
	char oldpiece[8];
	char newpiece[8];
	char *RetString=NULL;
	char *StringWithoutSomeChars=NULL;

	if (strcmp(Tag,"\r\n"))
	{
		/* Replace \r\n by \\r\\n */
		strcpy(oldpiece,"\r\n");
		strcpy(newpiece,"\\r\\n");
		StringWithoutSomeChars=ReplaceChars( Tag,oldpiece,newpiece);

		if (StringWithoutSomeChars)
		{
			RetString=SearchHashtable_string(Table,StringWithoutSomeChars);
			if (StringWithoutSomeChars) FREE(StringWithoutSomeChars);

			if (RetString)
			{
				/* Replace \\r\\n by \r\n */
				strcpy(oldpiece,"\\r\\n");
				strcpy(newpiece,"\r\n");
				StringWithoutSomeChars=ReplaceChars(RetString,oldpiece,newpiece);
				if (RetString) FREE(RetString);
				RetString = StringWithoutSomeChars;
			}
		}
	}
	return RetString;
}
/*-----------------------------------------------------------------------------------*/ 
