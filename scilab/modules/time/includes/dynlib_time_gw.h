/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __DYNLIB_TIME_GW_H__
#define __DYNLIB_TIME_GW_H__

#ifdef _MSC_VER
#if TIME_GW_EXPORTS
#define EXTERN_TIME_GW __declspec (dllexport)
#else
#define EXTERN_TIME_GW __declspec (dllimport)
#endif
#else
#define EXTERN_TIME_GW
#endif

#endif /* !__DYNLIB_TIME_GW_H__ */