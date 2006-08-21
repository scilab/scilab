/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET*/
/*-----------------------------------------------------------------------------------*/ 
#include <windows.h>
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
	#ifdef EXPORT_DETECTFRAMEWORK_DLL
		#define IMPORT_EXPORT_DETECTFRAMEWORK_DLL __declspec(dllexport)
	#else
		#define IMPORT_EXPORT_DETECTFRAMEWORK_DLL __declspec(dllimport)
	#endif
#else
	#define IMPORT_EXPORT_DETECTFRAMEWORK_DLL   extern
#endif

#ifdef _MSC_VER
	#define IMPORT_DLL __declspec(dllimport)
#else
	#define IMPORT_DLL   extern
#endif

/*-----------------------------------------------------------------------------------*/ 
#ifndef __DETECTFRAMEWORK__
#define __DETECTFRAMEWORK__

IMPORT_EXPORT_DETECTFRAMEWORK_DLL int DetectFrameWorkNET2(void);

#endif /* __DETECTFRAMEWORK__ */

/*-----------------------------------------------------------------------------------*/ 
