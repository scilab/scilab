/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __EXPORT_SYMBOL_H__
#define __EXPORT_SYMBOL_H__

#ifdef _MSC_VER
#pragma warning(disable : 4251) //It's not beautifull but that works !
#endif

#ifdef _MSC_VER
#if SYMBOL_EXPORTS
#define EXTERN_SYMBOL __declspec (dllexport)
#else
#define EXTERN_SYMBOL __declspec (dllimport)
#endif
#else
#define EXTERN_SYMBOL
#endif

#endif /* __EXPORT_SYMBOL_H__ */
