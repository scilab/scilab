/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 2006 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __EXPORTIMPORT_MALLOC_H__
#define __EXPORTIMPORT_MALLOC_H__

/* required on Windows to export or import symbols from a dynamic library */

#ifdef _MSC_VER
	#ifdef EXPORT_MALLOC_DLL
		#define IMPORT_EXPORT_MALLOC_DLL __declspec(dllexport)
	#else
		#define IMPORT_EXPORT_MALLOC_DLL __declspec(dllimport)
	#endif
#else
	#define IMPORT_EXPORT_MALLOC_DLL extern
#endif

#endif /* __EXPORTIMPORT_MALLOC_H__ */
/*-----------------------------------------------------------------------------------*/
