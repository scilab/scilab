/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/ 
#ifndef __XMLENCODING_H__
#define __XMLENCODING_H__

#include <wchar.h>

/* routines used to convert LibXml string */

/**
* convert a string to wide char encoding
* @param[in] UTF
* @return wide char
*/
wchar_t *toWideString(char *_Str);

/**
* convert a wide string to UTF
* @param[in] wide string
* @return  UTF
*/
char *toUTF(wchar_t *_wcStr);

#endif /* __TOWIDESTRING_H__ */
/*--------------------------------------------------------------------------*/ 
