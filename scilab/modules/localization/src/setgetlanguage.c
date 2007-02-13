/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "setgetlanguage.h"
#include "MALLOC.h"
#include "tableslanguages.h"
/*-----------------------------------------------------------------------------------*/ 
static char CURRENTLANGUAGESTRING[LengthAlphacode]=SCILABDEFAULTLANGUAGE;
static int  CURRENTLANGUAGECODE=SCILABDEFAULTLANGUAGECODE;
/*-----------------------------------------------------------------------------------*/ 
static BOOL LanguageIsOK(char *lang);
static int FindLanguageCode(char *lang);
static BOOL setlanguagecode(char *lang);
static char *FindAlias(char *lang);
static char *GetLanguageFromAlias(char *langAlias);
/*-----------------------------------------------------------------------------------*/ 
BOOL setlanguage(char *lang)
{
	BOOL bOK=FALSE;
	if (lang)
	{
		char LANGUAGETMP[LengthAlphacode];
		if (strlen(lang)==2)
		{
			char *ptrLang=NULL;
			ptrLang=GetLanguageFromAlias(lang);
			if (ptrLang)
			{
				strcpy(LANGUAGETMP,ptrLang);
				FREE(ptrLang);
				ptrLang=NULL;
			}
		}
		else if (strcmp(lang,"eng")==0) /* compatibility previous scilab */
		{
			char *ptrLang=NULL;
			ptrLang=GetLanguageFromAlias("en");
			if (ptrLang)
			{
				strcpy(LANGUAGETMP,ptrLang);
				FREE(ptrLang);
				ptrLang=NULL;
			}
		}
		else
		{
			strcpy(LANGUAGETMP,lang);
		}

		if ( LanguageIsOK(LANGUAGETMP) )
		{
			strcpy(CURRENTLANGUAGESTRING,LANGUAGETMP);
			setlanguagecode(LANGUAGETMP);
			bOK=TRUE;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
char *getlanguage(void)
{
	char *RetLanguage=NULL;

	RetLanguage=(char*)MALLOC(sizeof(char)*(strlen(CURRENTLANGUAGESTRING)+1));
	strcpy(RetLanguage,CURRENTLANGUAGESTRING);

	return RetLanguage;
}
/*-----------------------------------------------------------------------------------*/ 
int getcurrentlanguagecode(void)
{
	return CURRENTLANGUAGECODE;
}
/*-----------------------------------------------------------------------------------*/ 
static BOOL LanguageIsOK(char *lang)
{
	BOOL bOK=FALSE;
	int i=0;

	for (i=0;i<NumberLanguages;i++)
	{
		if (strcmp(lang,LANGUAGE_COUNTRY_TAB[i].alphacode)==0)
		{
			bOK=TRUE;
			return bOK;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
static int FindLanguageCode(char *lang)
{
	int Code=-1;
	int i=0;

	for (i=0;i<NumberLanguages;i++)
	{
		if (strcmp(lang,LANGUAGE_COUNTRY_TAB[i].alphacode)==0)
		{
			Code=LANGUAGE_COUNTRY_TAB[i].code;
			return Code;
		}
	}
	return Code;
}
/*-----------------------------------------------------------------------------------*/ 
static BOOL setlanguagecode(char *lang)
{
	BOOL bOK=FALSE;
	int tmpCode=FindLanguageCode(lang);
	
	if (tmpCode>0) 
	{
		CURRENTLANGUAGECODE=tmpCode;
		bOK=TRUE;
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
static char *FindAlias(char *lang)
{
	char *RetAlias=NULL;
	int i=0;
	for(i=0;i<NumberLanguagesAlias;i++)
	{
		if (strcmp(LANGUAGE_LOCALE_ALIAS[i].alphacode,lang)==0)
		{
			RetAlias=(char*)MALLOC(sizeof(char)*(strlen(LANGUAGE_LOCALE_ALIAS[i].alias)+1));
			strcpy(RetAlias,LANGUAGE_LOCALE_ALIAS[i].alias);
			return RetAlias;
		}
	}
	return RetAlias;
}
/*-----------------------------------------------------------------------------------*/ 
static char *GetLanguageFromAlias(char *langAlias)
{
	char *RetLanguage=NULL;
	int i=0;
	for(i=0;i<NumberLanguagesAlias;i++)
	{
		if (strcmp(LANGUAGE_LOCALE_ALIAS[i].alias,langAlias)==0)
		{
			RetLanguage=(char*)MALLOC(sizeof(char)*(strlen(LANGUAGE_LOCALE_ALIAS[i].alphacode)+1));
			strcpy(RetLanguage,LANGUAGE_LOCALE_ALIAS[i].alphacode);
			return RetLanguage;
		}
	}
	return RetLanguage;
}
/*-----------------------------------------------------------------------------------*/ 
char *getlanguagealias(void)
{
	return FindAlias(CURRENTLANGUAGESTRING);
}
/*-----------------------------------------------------------------------------------*/ 
