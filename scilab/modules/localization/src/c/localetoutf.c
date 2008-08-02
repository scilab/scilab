/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - Yung-Jang Lee
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <iconv.h>         /* use iconv library for UTF-8 converter */

#include "stack-def.h" /* bsiz */
#include "stricmp.h"
#include "localetoutf.h"
#ifndef _MSC_VER
	#include <unistd.h>
	#include <errno.h>
#else
	#include "strdup_windows.h"
#endif
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
iconv_t localeToUTFConvert = (iconv_t)-1; /* initialize to -1,  */

#define MAXPRINTF bsiz /* bsiz size of internal chain buf */

BOOL unicodeSubset = TRUE; /* if charset is subset of unicode, 
							no need to convert */
/*--------------------------------------------------------------------------*/
/* Convert from locale to UTF-8 */
void localeToUTF(char** buffer) 
{
	static char UTF_buf[MAXPRINTF];
	size_t inbytesleft = strlen(*buffer);
	size_t outbytesleft = MAXPRINTF;
	char *inPtr = *buffer;
	char *outPtr = UTF_buf;

	if(unicodeSubset) return; /*no need to convert for unicode subset encoding*/

	if (iconv (localeToUTFConvert, (const char**)&inPtr,&inbytesleft, &outPtr, &outbytesleft) == (size_t)(-1))
	{
		fprintf(stderr, "Error during call to iconv: %s\n", strerror(errno));
		fprintf(stderr, "String Input: %s\n", inPtr);
		return;
	}
	*outPtr = '\0';
	*buffer = UTF_buf;
	return;
}
/*--------------------------------------------------------------------------*/
void openLocaleToUTFConverter(char *sysLocale,char *lang)
{
    /* Assume sysLocale in lang_contry.charset or lang_contry format
	 *
	 * Ex. en_US, zh_TW.CP950, zh_CN.UTF-8
	 */
	char *encoding = NULL;

	if (sysLocale == NULL) return ; 

	encoding = getEncoding(sysLocale);
  
	unicodeSubset = TRUE; /* default */

#ifdef _MSC_VER
	/*  Under Windows only Code page 1252 (iso-8859-1) is subset of UNICODE
		http://www.science.co.il/Language/Character-Sets.asp */
	if ( stricmp("CP1252", encoding) !=0 ) unicodeSubset = FALSE;
#else
	/* Under other OS, convert for encoding not in utf-8 format*/
	if ( stricmp("utf-8", encoding) !=0 && stricmp("utf8", encoding)!=0 && stricmp("", encoding) !=0 )
	{
		unicodeSubset = FALSE;
	}
#endif


	/* if not utf-8 encoding and multi-byte language ..*/
	if ( ! unicodeSubset)
	{ 
		/* need locale to utf convert */
		if(localeToUTFConvert !=(iconv_t)-1)      
		{
			/*if already open*/ 
			iconv_close(localeToUTFConvert); /* close iconv server */
		}

		localeToUTFConvert = iconv_open("UTF-8",encoding);     /* open iconv server :from locale to UTF8 */

		if (localeToUTFConvert==(iconv_t) -1)
		{
			fprintf(stderr, "Error during call to iconv_open: %s\nCharset encoding %s\n", strerror(errno),encoding);
		}
	} 
	else 
	{ 
		/* subset of unicode , no need to convert*/
 		if (localeToUTFConvert != (iconv_t)-1) 
		{
			iconv_close(localeToUTFConvert); /* close any exist iconv server */
		}
		localeToUTFConvert=(iconv_t)-1;
	}

	if (encoding) {FREE(encoding); encoding = NULL;}
}
/*--------------------------------------------------------------------------*/
char *getEncoding(char *lang)
{
	char *encoding = NULL;
	if (lang)
	{
		char *pch = strchr(lang, '.'); /* for example, if we have zh_TW.UTF8 */
		if (pch)
		{    
			encoding = strdup(pch+1); /* encoding */
		}
		else
		{
			/*in "lang_contry" format*/
			if(stricmp("zh_TW",lang) ==0) 
			{
				encoding = strdup("BIG5");
			}
			else
			if(stricmp("ru_RU",lang) ==0)
			{
				encoding = strdup("ISO-8859-5");
			}
			else
			{
				/*default to UTF8*/
				encoding = strdup("UTF-8"); 
			}
		}
	}
	return encoding;
}
/*--------------------------------------------------------------------------*/
