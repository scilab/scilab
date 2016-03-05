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
