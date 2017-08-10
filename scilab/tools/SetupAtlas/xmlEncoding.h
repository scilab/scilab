/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
