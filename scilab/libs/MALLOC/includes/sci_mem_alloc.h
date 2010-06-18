/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - 2005 - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 

#ifndef __SCI_MEM_ALLOC__
#define __SCI_MEM_ALLOC__

#undef Top

#ifdef _MSC_VER
#error This file should not be used on windows
#else
#include <stddef.h> /* for size_t */
#endif

#ifdef MALLOC
#undef MALLOC
#endif

#ifdef FREE
#undef FREE
#endif

#ifdef CALLOC
#undef CALLOC
#endif

#ifdef REALLOC
#undef REALLOC
#endif

#ifdef NULL
	#undef NULL
#endif
#define NULL 0

#include "ExportImport.h" /* IMPORT_EXPORT_MALLOC_DLL */

/**
 * Reallocate the memory
 * Unix/Linux only
 *
 * @param lpAddress Address 
 * @param dwSize Size
 * @param file Which file is calling the function (used debug)
 * @param line Which line is calling the function (used debug)
 * @return <ReturnValue>
 */
 void * MyReAlloc(void * lpAddress,size_t dwSize,const char *file,int line);
 
 /**
 * Allocate the memory
 * Unix/Linux only
 *
 * @param dwSize Size 
 * @param file Which file is calling the function (used debug) 
 * @param line Which line is calling the function (used debug) 
 * @return <ReturnValue>
 */
 void * MyAlloc(size_t dwSize,const char *file,int line);
 
   /**
 * 
 * Unix/Linux only
 *
 * @param x
 * @param y
 * @param file Which file is calling the function (used debug) 
 * @param line Which line is calling the function (used debug) 
 * @return <ReturnValue>
 */
 void * MyCalloc(size_t x, size_t y, const char *file,int line);
 
  /**
 * Free the memory
 * Unix/Linux only
 *
 * @param lpAddress Address 
 * @return <ReturnValue>
 */
void MyFree(void *lpAddress);

#define MALLOC(x) MyAlloc((size_t)x,__FILE__,__LINE__)
#define FREE(x) MyFree((void *) x)
#define REALLOC(x,y) MyReAlloc((void *)x, y,__FILE__,__LINE__)
#define CALLOC(x,y) MyCalloc((size_t) x, (size_t) y,__FILE__,__LINE__)

/* Only for Scilab Stacksize use VirtualAlloc on Window */
#define SCISTACKMALLOC(x) MyAlloc((unsigned long)x,__FILE__,__LINE__)
#define SCISTACKFREE(x) MyFree((void *) x)

#define Top C2F(vstk).top

#endif /* __SCI_MEM_ALLOC__ */
