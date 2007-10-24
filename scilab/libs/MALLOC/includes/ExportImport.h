/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
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
