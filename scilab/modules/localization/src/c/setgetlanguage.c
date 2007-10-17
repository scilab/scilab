/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include <stdlib.h>
#include "setgetlanguage.h"
#include "MALLOC.h"
#include "tableslanguages.h"
#include "loadsavelanguage.h"
#include "syncexec.h"
/*-----------------------------------------------------------------------------------*/ 
static char CURRENTLANGUAGESTRING[LengthAlphacode]=SCILABDEFAULTLANGUAGE;
static int  CURRENTLANGUAGECODE=SCILABDEFAULTLANGUAGECODE;
/*-----------------------------------------------------------------------------------*/ 
static int FindLanguageCode(char *lang);
static BOOL setlanguagecode(char *lang);
static char *FindAlias(char *lang);
static char *GetLanguageFromAlias(char *langAlias);
/*-----------------------------------------------------------------------------------*/ 
BOOL setlanguage(char *lang,BOOL updateHelpIndex, BOOL updateMenus)
{
	BOOL bOK=FALSE;
	if (lang)
	{
		if ( LanguageIsOK(lang) )
		{
			if (needtochangelanguage(lang))
			{
				/* change language */
				strcpy(CURRENTLANGUAGESTRING,lang);
				setlanguagecode(lang);

				if (updateHelpIndex)
				{
					#define UPDATESCILABHELPMACRO "try update_scilab_help();catch end;clear update_scilab_help;" 
					integer ierr ;
					integer seq = 1 ;
					int macroCallLength=0;

					/* update help index */
					macroCallLength = (int)strlen(UPDATESCILABHELPMACRO);
					C2F(syncexec)(UPDATESCILABHELPMACRO,&macroCallLength,&ierr,&seq, macroCallLength);
				}

				/* save language pref. */
				savelanguagepref();

				if (updateMenus)
				{
					/* changes menus : to do after */
				}
				bOK=TRUE;
			}
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
char *getlanguagefromcode(int code)
{
	char *RetLanguage=NULL;
	int i=0;

	for (i=0;i<NumberLanguages;i++)
	{
		if (LANGUAGE_COUNTRY_TAB[i].code == code)
		{
			RetLanguage=(char*)MALLOC(sizeof(char)*(strlen(LANGUAGE_COUNTRY_TAB[i].alphacode)+1));
			strcpy(RetLanguage,LANGUAGE_COUNTRY_TAB[i].alphacode);
			return RetLanguage;
		}
	}
	return RetLanguage;
}
/*-----------------------------------------------------------------------------------*/ 
int getcodefromlanguage(char *language)
{
	return FindLanguageCode(language);
}
/*-----------------------------------------------------------------------------------*/ 
BOOL LanguageIsOK(char *lang)
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
int comparelanguages(char *language1,char *language2)
{
	return strcmp(language1,language2);
}
/*-----------------------------------------------------------------------------------*/ 
BOOL needtochangelanguage(char *language)
{
	BOOL bOK=FALSE;
	char *currentlanguage=NULL;

	currentlanguage=getlanguage();

	if (comparelanguages(language,currentlanguage)) bOK=TRUE;

	if (currentlanguage) {FREE(currentlanguage);currentlanguage=NULL;}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
char *convertlanguagealias(char *strlanguage)
{
	char *correctlanguage=NULL;

	if (strlen(strlanguage)==2)
	{
		correctlanguage=GetLanguageFromAlias(strlanguage);
	}
	else if (strcmp(strlanguage,"eng")==0) /* compatibility previous scilab */
	{
		correctlanguage=GetLanguageFromAlias("en");
	}
	return correctlanguage;
}
/*-----------------------------------------------------------------------------------*/ 
