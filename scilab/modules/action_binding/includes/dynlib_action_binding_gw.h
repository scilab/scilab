/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2011 - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#ifndef __DYNLIB_ACTION_BINDING_GW_H__
#define __DYNLIB_ACTION_BINDING_GW_H__

#ifdef _MSC_VER
#ifdef ACTION_BINDING_GW_EXPORTS
#define ACTION_BINDING_GW_IMPEXP __declspec(dllexport)
#else
#define ACTION_BINDING_GW_IMPEXP __declspec(dllimport)
#endif
#else
#define ACTION_BINDING_GW_IMPEXP
#endif

#endif /* __DYNLIB_ACTION_BINDING_GW_H__ */
/*--------------------------------------------------------------------------*/
