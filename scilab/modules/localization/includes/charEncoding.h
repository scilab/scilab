/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#ifndef __CHARENCODING_H__
#define __CHARENCODING_H__

#include "BOOL.h"

/**
* Set if the output must be in UTF or not (many used by do_xxprintf.c
* set_xxorintf.c)
*
*/
void setOutputInUTF(BOOL);

/**
* Return is the output is in UTF 8 or not
*
* @return TRUE is UTF FALSE otherwise
*/
BOOL isOutputInUTF(void);

/**
* Return the encoding of a specific lang
*
* @param lang  the langage
* @return the encoding
*/
char *getEncoding(char *lang);

/**
* @TODO add comment
*
* @param buffer
* @return <ReturnValue>
*/
char* UTFToLocale(char* _szBufferIn, char *_szBuffOut);

char* localeToUTF(char* _szBufferIn, char *_szBuffOut);

void openCharEncodingConverter(char *encoding);

void closeCharEncodingConverter(void);

char * UTFToConsole(char* _szLineIn, char* _szLineOut);

char* readNextUTFChar(char* utfstream,int* size);


#endif /* __CHARENCODING_H__ */

