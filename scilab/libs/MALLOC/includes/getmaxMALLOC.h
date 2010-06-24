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
#ifndef __GETMAXMALLOC_H__
#define __GETMAXMALLOC_H__
/*-----------------------------------------------------------------------------------*/ 
#include "ExportImport.h" /* IMPORT_EXPORT_MALLOC_DLL */

IMPORT_EXPORT_MALLOC_DLL unsigned long GetLargestFreeMemoryRegion(void);

#endif /* __GETMAXMALLOC_H__ */
/*-----------------------------------------------------------------------------------*/ 

