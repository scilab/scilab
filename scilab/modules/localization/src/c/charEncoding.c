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
#include <libintl.h>

#include "scilabmode.h"
#include "stack-def.h" /* bsiz */
#include "stricmp.h"
#include "BOOL.h"
#include "charEncoding.h"
#ifndef _MSC_VER
#include <unistd.h>
#include <errno.h>
#else
#include "strdup_windows.h"
#endif
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/

static iconv_t localeToUTFConvert = (iconv_t)-1; /* initialize to -1,  */
static iconv_t UTFToLocaleConvert = (iconv_t)-1; /* initialize to -1,  */


BOOL unicodeSubset = TRUE; /* if charset is subset of unicode, no need to convert */
/*--------------------------------------------------------------------------*/
#define ENCODE_BUF_SIZE  bsiz /* bsiz size of internal chain buf */
//static char ENCODE_BUF1[ENCODE_BUF_SIZE]; // the first buffer to store the converted string
//static char ENCODE_BUF2[ENCODE_BUF_SIZE]; // the second buffer  to store the converted string
//static char* ENCODE_BUF=ENCODE_BUF1; // pointer to the next buffer for the converted string
//static char* _CharVec[255] ;    // Global pointers to point the converted UTF-8 or locale strings (multiple lines)

/*--------------------------------------------------------------------------*/
char *getEncoding(char *lang)
{
	static char encoding[125];
	if (lang)
	{
		char *pch = strchr(lang, '.'); /* for example, if we have zh_TW.UTF8 */
		if (pch)
		{
			strcpy(encoding,pch+1);
		}
		else // if encoding not explicitly given
		{
			/* in "lang_contry" format */
			if(stricmp("zh_TW",lang) ==0)  //chinese traditional
			{
				strcpy(encoding,"BIG5");
			}
			else if(stricmp("zh_CN",lang) ==0)//chinese simplified
			{
				strcpy(encoding,"GB2312");
			}
			else if(stricmp("ru_RU",lang) ==0) //russian
			{
				strcpy(encoding,"ISO-8859-5");
			}
			else if((stricmp("ja_JP",lang) ==0) ||(stricmp("jp_JP",lang) ==0)  ) //japaness
			{
				strcpy(encoding,"eucJP");
			}
			else if((stricmp("ko",lang) ==0) ||(stricmp("ko_KR",lang) ==0)  ) //korean
			{
				strcpy(encoding,"eucKR");
			}
			else
			{
				/* default to ISO8859-1, LATIN-1 languages*/
				/* under linux language_countryXX.ISO8859-1 */
				strcpy(encoding,"ISO8859-1");
			}
		}
	}
	else
	{
		/*default to ISO8859-1, LATIN-1 languages*/
		strcpy(encoding,"ISO8859-1");
	}
	return encoding;
}


char* localeToUTF(char* _szBufferIn, char* _szBufferOut) 
{
	size_t inbytesleft = 0;
	size_t outbytesleft = bsiz;
	char *inPtr = _szBufferIn;
	char *outPtr = _szBufferOut;

	return _szBufferIn;


	/* no need to convert for unicode subset encoding*/
	if(unicodeSubset)
	{
		return _szBufferIn;
	}
	if (_szBufferIn == NULL)
	{
		return NULL;
	}

	inbytesleft = strlen(_szBufferIn);


	if (iconv (localeToUTFConvert, (const char**)&inPtr,&inbytesleft, &outPtr, &outbytesleft) == (size_t)(-1))
	{
		fprintf(stderr, "Error during call to localeToUTF: %s\n", strerror(errno));
		fprintf(stderr, "String Input: %s\n", inPtr);
		return _szBufferIn; // return unconverted text
	}

	*outPtr='\0';
	return _szBufferOut;
}

char* UTFToLocale(char* _szBufferIn, char* _szBufferOut)
{
	size_t inbytesleft = 0;
	size_t outbytesleft = bsiz;
	char *inPtr = _szBufferIn;
	char *outPtr= _szBufferOut;

	return _szBufferIn;


	inbytesleft = strlen(_szBufferIn) + 1;

	/* no need to convert for unicode subset encoding*/
	if(unicodeSubset)
	{
		return _szBufferIn;
	}

	if (_szBufferIn == NULL)
	{
		return NULL;
	}

	if (iconv (UTFToLocaleConvert, (const char**)&inPtr,&inbytesleft, &outPtr, &outbytesleft) == (size_t)(-1))
	{
		fprintf(stderr, "Error during call to UTFToLocale: %s\n", strerror(errno));
		fprintf(stderr, "String Input: %s\n", inPtr);
		return _szBufferIn;//return unconverted text
	}
	*outPtr='\0';
	return _szBufferOut;
}
/*--------------------------------------------------------------------------*/


void openCharEncodingConverter(char *encoding)
{

	unicodeSubset = TRUE; /* default */
#ifdef _MSC_VER
	/* Under  Windoes every code page  is not subset of UNICODE (including CP1252), it means  we always need
	a  converter */
	unicodeSubset = FALSE; /* default */
#else
	/* Under other OS, we need converter for encoding not in utf-8 format*/
	if(stricmp("utf-8", encoding) !=0 && stricmp("utf8", encoding)!=0 && stricmp("", encoding) !=0)
	{
		unicodeSubset = FALSE;
	}
#endif

	/* if not utf-8 encoding and multi-byte language ..*/
	if(!unicodeSubset)
	{
		/* need locale to utf convert */
		if(localeToUTFConvert !=(iconv_t)-1) 
		{
			iconv_close(localeToUTFConvert); /* close iconv localeToUTF server */
		}

		if(UTFToLocaleConvert !=(iconv_t)-1) 
		{
			iconv_close(UTFToLocaleConvert); /* close iconv UTFToLocale server */
		}



		localeToUTFConvert = iconv_open("UTF-8",encoding);     /* open iconv server :from locale to UTF8 */
		if (localeToUTFConvert==(iconv_t) -1)
		{
			fprintf(stderr, "Error during call to iconv_open for locale to UTF converter: %s\nCharset encoding %s\n", strerror(errno),encoding);
		}

		UTFToLocaleConvert = iconv_open(encoding,"UTF-8");     /* open iconv server :from UTF8  to locale */
		if (UTFToLocaleConvert==(iconv_t) -1) 
		{
			fprintf(stderr, "Error during call to iconv_open for UTF to locale converter: %s\nCharset encoding %s\n", strerror(errno),encoding);
		}
	}
	else 
	{
		closeCharEncodingConverter();
	}
}

void closeCharEncodingConverter(void)
{
	if (localeToUTFConvert != (iconv_t)-1) 
	{
		iconv_close(localeToUTFConvert); /* close any exist iconv server */
	}

	localeToUTFConvert=(iconv_t)-1;
	if (UTFToLocaleConvert != (iconv_t)-1) 
	{
		iconv_close(UTFToLocaleConvert); /* close any exist iconv server */
	}

	UTFToLocaleConvert=(iconv_t)-1;
}

/*--------------------------------------------------------------------------*/



//
// getScilabMode()  =  SCILAB_API) || SCILAB_STD) || (newmode == SCILAB_NW) || (newmode == SCILAB_NWNI)
//

char * UTFToConsole(char* _szLineIn, char* _szLineOut)
{
	if (getScilabMode() == SCILAB_STD) 
	{
		return _szLineIn; // String in Java console mode already in UTF
	} 
	else
	{
		return UTFToLocale(_szLineIn, _szLineOut); // Terminal mode
	}
}

char* readNextUTFChar(char* utfstream,int* size)
{
	static char UTFChar[5]; /**UTF char. at most 4 bytes*/
	unsigned char charcode=(unsigned)*utfstream;
	/** UTF-8 format: ref. http://en.wikipedia.org/wiki/UTF-8/ */
	if(charcode > 193 && charcode <= 223 )
	{ /* twi bytes UTF-8 */
		UTFChar[0]=*utfstream;
		UTFChar[1]=*(utfstream+1);
		UTFChar[2]='\0';
		*size=2;
	}
	else if(charcode > 223 && charcode <= 239 ) 
	{/* three bytes UTF-8*/
		UTFChar[0]=*utfstream;
		UTFChar[1]=*(utfstream+1);
		UTFChar[2]=*(utfstream+2);;
		UTFChar[3]='\0';
		*size=3;
	}
	else if(charcode > 239 && charcode < 245 ) 
	{/* four bytes UTF-8*/
		UTFChar[0]=*utfstream;
		UTFChar[1]=*(utfstream+1);
		UTFChar[2]=*(utfstream+2);
		UTFChar[3]=*(utfstream+3);
		UTFChar[4]='\0';
		*size=4;
	}
	else 
	{
		UTFChar[0]=*utfstream;
		UTFChar[1]='\0';
		*size=1;
	}
	return UTFChar;
}



static BOOL outputInUTFEncoding = TRUE;
/* Only used inside do_xxprintf.c set_xxorintf.c*/

void setOutputInUTF(BOOL isUTF) 
{
	outputInUTFEncoding=isUTF;
}

BOOL isOutputInUTF() 
{
	return outputInUTFEncoding;
}
