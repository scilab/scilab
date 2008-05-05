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

/**
* convert a ANSI string to UTF-8 string (Windows Encoding)
* @param[in] ANSI string
* @return UTF-8 string
*/
char * ANSIToUTF8(char * pszCode);

/**
* convert a UTF-8 string to ANSI string (Windows Encoding)
* @param[in] UTF-8 string
* @return ANSI string
*/
char* UTF8ToANSI(char *pszCode);