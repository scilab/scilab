/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
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
#ifndef __DYNLIB_ELEMENTARY_FUNCTIONS_H__
#define __DYNLIB_ELEMENTARY_FUNCTIONS_H__

#ifdef _MSC_VER
#ifdef ELEMENTARY_FUNCTIONS_EXPORTS
#define ELEMENTARY_FUNCTIONS_IMPEXP __declspec(dllexport)
#else
#define ELEMENTARY_FUNCTIONS_IMPEXP __declspec(dllimport)
#endif
#else
#define ELEMENTARY_FUNCTIONS_IMPEXP
#endif

#endif /* __DYNLIB_ELEMENTARY_FUNCTIONS_H__ */
/*--------------------------------------------------------------------------*/
