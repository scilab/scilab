/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#ifndef __DYNLIB_SPARSE_GW_H__
#define __DYNLIB_SPARSE_GW_H__

#ifdef _MSC_VER
#ifdef SPARSE_GW_EXPORTS
#define SPARSE_GW_IMPEXP __declspec(dllexport)
#else
#define SPARSE_GW_IMPEXP __declspec(dllimport)
#endif
#else
#define SPARSE_GW_IMPEXP
#endif

#endif /* __DYNLIB_SPARSE_GW_H__ */
/*--------------------------------------------------------------------------*/
