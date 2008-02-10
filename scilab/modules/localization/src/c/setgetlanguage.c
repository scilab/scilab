/*--------------------------------------------------------------------------*/
/* INRIA 2007/2008 */
/* @author Allan CORNET */
/* @author Sylvestre LEDRU */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "machine.h"

#ifndef _MSC_VER
 #ifdef HAVE_LIBINTL_H
  #include <libintl.h>
  #ifdef HAVE_LOCALE_H
   #include <locale.h>
  #else
   #error "Cannot find locale.h despite that libintl.h is available"
  #endif
 #endif
#else
	#include <locale.h>
#endif

#include "setgetlanguage.h"
#include "MALLOC.h"
#include "tableslanguages.h"
#include "defaultlanguage.h"
#include "loadsavelanguage.h"
#include "syncexec.h"
#include "scilabDefaults.h"
#include "../../../io/includes/setenvc.h"

/*--------------------------------------------------------------------------*/
static char CURRENTLANGUAGESTRING[LengthAlphacode]=SCILABDEFAULTLANGUAGE;
static int  CURRENTLANGUAGECODE=SCILABDEFAULTLANGUAGECODE;
/*--------------------------------------------------------------------------*/
static int FindLanguageCode(char *lang);
static BOOL setlanguagecode(char *lang);
static char *FindAlias(char *lang);
static void putEnvLC_ALL(char *locale);
static char *GetLanguageFromAlias(char *langAlias);

#ifdef _MSC_VER
/**
* returns user locale string
* @return Locale user example fr_FR or en_US
*/
static char* getLocaleUserInfo(void);

/**
* returns sytem locale string
* @return Locale system example fr_FR or en_US
*/
static char* getLocaleSystemInfo(void);
#endif
/*--------------------------------------------------------------------------*/
BOOL setlanguage(char *lang,BOOL updateHelpIndex, BOOL updateMenus)
{
	if (lang)
	{
		if (LanguageIsOK(lang))
		{
			if (needtochangelanguage(lang))
			{
				char *ret=NULL;

				/* Load the locale from the system */
#ifndef _MSC_VER
				ret=setlocale(LC_MESSAGES,lang);
#else
				/* MS VS (setlocale) doesn't know LC_MESSAGES */
				/* http://msdn2.microsoft.com/en-us/library/x99tb11d(vs.71).aspx */
				ret = setlocale(LC_CTYPE,lang);
#endif
				/*
				  This stuff causes pb when locales have been compiled 
				if (ret==NULL){
					fprintf(stderr, "Localization: Doesn't support the locale '%s'.\n",lang);
					return FALSE;
				}
				*/

				putEnvLC_ALL(lang);

				/* change language */
				if (strcmp(lang,"C")==0){
					/* The lang is the default one... ie en_US */
					strcpy(CURRENTLANGUAGESTRING,"en_US");
				}else{
					if (strcmp(lang,"")==0 && ret!=NULL){
						/* The requested language is the one of the system ... 
						 * which we don't really know which one is it
						 * but if setlocale worked, we get it from the return 
						 */
						strncpy(CURRENTLANGUAGESTRING,ret,5); /* 5 is the number of char in fr_FR for example */
					}else{
						strcpy(CURRENTLANGUAGESTRING,lang);
					}
				}
				setlanguagecode(CURRENTLANGUAGESTRING);

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
				return TRUE;
			}
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
char *getlanguage(void)
{
	#if _MSC_VER
	return getLocaleUserInfo();
	#else
	return CURRENTLANGUAGESTRING;
	#endif
	
}
/*--------------------------------------------------------------------------*/
int getcurrentlanguagecode(void)
{
	return CURRENTLANGUAGECODE;
}
/*--------------------------------------------------------------------------*/
char *getlanguagefromcode(int code)
{
	int i=0;

	for (i=0;i<NumberLanguages;i++)
	{
		if (LANGUAGE_COUNTRY_TAB[i].code == code)
		{
			return LANGUAGE_COUNTRY_TAB[i].alphacode;
		}
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
int getcodefromlanguage(char *language)
{
	return FindLanguageCode(language);
}
/*--------------------------------------------------------------------------*/
BOOL LanguageIsOK(char *lang)
{
	int i=0;

	if (strlen(lang)==0){ /* Empty language declaration... it is the default 
						   * language from the system */
		return TRUE;
	}

	for (i=0;i<NumberLanguages;i++)
	{
		if (strcmp(lang,LANGUAGE_COUNTRY_TAB[i].alphacode)==0)
		{
		  return TRUE;
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
char *getlanguagealias(void)
{
	return FindAlias(CURRENTLANGUAGESTRING);
}
/*--------------------------------------------------------------------------*/
BOOL needtochangelanguage(char *language)
{
  char *currentlanguage=getlanguage();

  if (strcmp(language,currentlanguage)) {
    return TRUE;
  }

  return FALSE;
}
/*--------------------------------------------------------------------------*/
char *convertlanguagealias(char *strlanguage)
{
	char *correctlanguage=NULL;

	if ( (strlen(strlanguage)==2) || (strcmp(strlanguage,"en_US")==0) ) /* If the user wants to change to en_US ... use the default locale */
	{
		correctlanguage=GetLanguageFromAlias(strlanguage);
	}
	else if (strcmp(strlanguage,"eng")==0) /* compatibility previous scilab */
	{
		correctlanguage=GetLanguageFromAlias("en");
	}
	return correctlanguage;
}
/*--------------------------------------------------------------------------*/
/**
 * Export the variable LC_ALL to the system
 *
 * @param locale the locale (ex : fr_FR or en_US)
 */
static void putEnvLC_ALL(char *locale){

	/* It will put in the env something like LC_ALL=fr_FR */
	if ( !setenvc(EXPORTENVLOCALE,locale))
	{
		fprintf(stderr,"Localization: Failed to declare the system variable LC_ALL\n");
	}
}
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static char* getLocaleUserInfo(void)
{
	#define LENGTH_BUFFER 1024
	char buffer_LOCALE_SISO639LANGNAME[LENGTH_BUFFER];
	char buffer_LOCALE_SISO3166CTRYNAME[LENGTH_BUFFER];
	char *localeStr = NULL;
	int ret = 0;
	ret = GetLocaleInfo(LOCALE_USER_DEFAULT,
						LOCALE_SISO639LANGNAME,
						&buffer_LOCALE_SISO639LANGNAME[0],
						LENGTH_BUFFER);
	if (ret > 0)
	{

		ret = GetLocaleInfo(LOCALE_USER_DEFAULT,
							LOCALE_SISO3166CTRYNAME,
							&buffer_LOCALE_SISO3166CTRYNAME[0],
							LENGTH_BUFFER);
		if (ret >0)
		{
			int length_localeStr = (int)(strlen(buffer_LOCALE_SISO639LANGNAME)+
										 strlen(buffer_LOCALE_SISO3166CTRYNAME)+
										 strlen("_"));
			localeStr = (char*)MALLOC(sizeof(char)*(length_localeStr)+1);
			if (localeStr)
			{
				#define FORMAT_LOCALE "%s_%s"
				sprintf(localeStr,FORMAT_LOCALE,buffer_LOCALE_SISO639LANGNAME,buffer_LOCALE_SISO3166CTRYNAME);
			}
		}
	}
	return localeStr;
}
#endif
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
static char* getLocaleSystemInfo(void)
{
	#define LENGTH_BUFFER 1024
	char buffer_LOCALE_SISO639LANGNAME[LENGTH_BUFFER];
	char buffer_LOCALE_SISO3166CTRYNAME[LENGTH_BUFFER];
	char *localeStr = NULL;
	int ret = 0;
	ret = GetLocaleInfo(LOCALE_SYSTEM_DEFAULT,
		LOCALE_SISO639LANGNAME,
		&buffer_LOCALE_SISO639LANGNAME[0],
		LENGTH_BUFFER);
	if (ret > 0)
	{
		ret = GetLocaleInfo(LOCALE_SYSTEM_DEFAULT,
			LOCALE_SISO3166CTRYNAME,
			&buffer_LOCALE_SISO3166CTRYNAME[0],
			LENGTH_BUFFER);
		if (ret >0)
		{
			int length_localeStr = (int)(strlen(buffer_LOCALE_SISO639LANGNAME)+
				strlen(buffer_LOCALE_SISO3166CTRYNAME)+
				strlen("_"));
			localeStr = (char*)MALLOC(sizeof(char)*(length_localeStr)+1);
			if (localeStr)
			{
				#define FORMAT_LOCALE "%s_%s"
				sprintf(localeStr,FORMAT_LOCALE,buffer_LOCALE_SISO639LANGNAME,buffer_LOCALE_SISO3166CTRYNAME);
			}
		}
	}
	return localeStr;
}
#endif
/*--------------------------------------------------------------------------*/
