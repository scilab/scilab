/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#ifndef __GETOS_H__
#define __GETOS_H__

#ifdef OSNAME
#undef OSNAME
#endif

#ifdef __APPLE__
#define OSNAME "macosx"
#else
#ifdef __linux__
#define OSNAME "linux"
#else
#ifdef _MSC_VER
#define OSNAME "windows"
#else
#define OSNAME "other"
#endif
#endif
#endif
/*--------------------------------------------------------------------------*/

/**
* get OS Full name based on uname
* @return a string
*/
char *getOSFullName(void);

/**
* get OS Release Info
* @return a string
*/
char *getOSRelease(void);

/*--------------------------------------------------------------------------*/
#endif /* __GETOS_H__ */
