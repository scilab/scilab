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

#ifndef __EXPORT_EF_H__
#define __EXPORT_EF_H__

#ifdef _MSC_VER
	#if __ELEM_FUNC__
		#define EXTERN_EF __declspec (dllexport)
	#else
		#define EXTERN_EF __declspec (dllimport)
	#endif
#else
	#define EXTERN_EF 
#endif

#endif /* __EXPORT_EF_H__ */