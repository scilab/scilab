/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __DYNLIB_SYSTEM_ENV_H__
#define __DYNLIB_SYSTEM_ENV_H__

#ifdef _MSC_VER
    #if SYSTEM_ENV_EXPORTS
        #define EXTERN_SYSTEM_ENV __declspec (dllexport)
    #else
        #define EXTERN_SYSTEM_ENV __declspec (dllimport)
    #endif
#else
    #define EXTERN_SYSTEM_ENV 
#endif

#endif // !__DYNLIB_SYSTEM_ENV_H__