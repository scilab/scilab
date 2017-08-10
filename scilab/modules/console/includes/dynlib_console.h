/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
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
#ifndef __DYNLIB_CONSOLE_H__
#define __DYNLIB_CONSOLE_H__

#ifdef _MSC_VER
#ifdef CONSOLE_EXPORTS
#define CONSOLE_IMPEXP __declspec(dllexport)
#else
#define CONSOLE_IMPEXP __declspec(dllimport)
#endif
#else
#define CONSOLE_IMPEXP
#endif

#endif /* __DYNLIB_CONSOLE_H__ */
/*--------------------------------------------------------------------------*/

