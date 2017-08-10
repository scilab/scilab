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
#ifndef __DYNLIB_CACSD_H__
#define __DYNLIB_CACSD_H__

#ifdef _MSC_VER
#ifdef CACSD_EXPORTS
#define CACSD_IMPEXP __declspec(dllexport)
#else
#define CACSD_IMPEXP __declspec(dllimport)
#endif
#else
#define CACSD_IMPEXP
#endif

#endif /* __DYNLIB_CACSD_H__ */
/*--------------------------------------------------------------------------*/
