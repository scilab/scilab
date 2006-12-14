/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GETMAXMALLOC_H__
#define __GETMAXMALLOC_H__
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
	#ifdef EXPORT_MALLOC_DLL
		#define IMPORT_EXPORT_MALLOC_DLL __declspec(dllexport)
	#else
		#define IMPORT_EXPORT_MALLOC_DLL __declspec(dllimport)
	#endif
#else
	#define IMPORT_EXPORT_MALLOC_DLL extern
#endif

IMPORT_EXPORT_MALLOC_DLL unsigned long GetLargestFreeMemoryRegion(void);

#endif /* __GETMAXMALLOC_H__
/*-----------------------------------------------------------------------------------*/ 

