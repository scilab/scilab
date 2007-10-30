/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include "setgetlanguage.h"
#include "MALLOC.h"
#include "tableslanguages.h"
#include "defaultlanguage.h"
#include "loadsavelanguage.h"
#include "syncexec.h"
/*-----------------------------------------------------------------------------------*/
//static LanguagesTable LANGUAGE_COUNTRY_TAB[NumberLanguages];
//static LanguagesTableAlias LANGUAGE_LOCALE_ALIAS[NumberLanguagesAlias];
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
	int i=0;

	for (i=0;i<NumberLanguages;i++)
	{
		if (strcmp(lang,LANGUAGE_COUNTRY_TAB[i].alphacode)==0)
		{
		  return TRUE;
		}
	}
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/
static int FindLanguageCode(char *lang)
{
	int i=0;

	for (i=0;i<NumberLanguages;i++)
	{
		if (strcmp(lang,LANGUAGE_COUNTRY_TAB[i].alphacode)==0)
		{
			return LANGUAGE_COUNTRY_TAB[i].code;
		}
	}
	return -1;
}
/*-----------------------------------------------------------------------------------*/
static BOOL setlanguagecode(char *lang)
{
	int tmpCode=FindLanguageCode(lang);

	if (tmpCode>0)
	{
		CURRENTLANGUAGECODE=tmpCode;
		return TRUE;
	}
	return FALSE;
}
/*-----------------------------------------------------------------------------------*/
static char *FindAlias(char *lang)
{
	int i=0;
	for(i=0;i<NumberLanguagesAlias;i++)
	{
		if (strcmp(LANGUAGE_LOCALE_ALIAS[i].alphacode,lang)==0)
		{
		  return LANGUAGE_LOCALE_ALIAS[i].alias;
		}
	}
	return NULL;
}
/*-----------------------------------------------------------------------------------*/
static char *GetLanguageFromAlias(char *langAlias)
{
	int i=0;
	for(i=0;i<NumberLanguagesAlias;i++)
	{
		if (strcmp(LANGUAGE_LOCALE_ALIAS[i].alias,langAlias)==0)
		{
			return LANGUAGE_LOCALE_ALIAS[i].alphacode;
		}
	}
	return NULL;
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
  char *currentlanguage=NULL;

  currentlanguage=getlanguage();

  if (comparelanguages(language,currentlanguage)) {
    if (currentlanguage) {
      FREE(currentlanguage);
      currentlanguage=NULL;
    }
    return TRUE;
  }

  return FALSE;
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
