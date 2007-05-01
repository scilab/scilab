/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2007 */
/* Allan CORNET*/
/*-----------------------------------------------------------------------------------*/ 
#ifndef __EXPORT_IMPORT_LIBRARY_JVM_H__
#define __EXPORT_IMPORT_LIBRARY_JVM_H__

#ifdef _MSC_VER
	#ifdef EXPORT_LIBJVM_DLL
		#define IMPORT_EXPORT_LIBJVM_DLL __declspec(dllexport)
	#else
		#define IMPORT_EXPORT_LIBJVM_DLL __declspec(dllimport)
	#endif
#else
	#define IMPORT_EXPORT_LIBJVM_DLL  extern
#endif

#ifdef _MSC_VER
	#define IMPORT_DLL __declspec(dllimport)
#else
	#define IMPORT_DLL   extern
#endif

#endif /* __EXPORT_IMPORT_LIBRARY_JVM_H__ */
/*-----------------------------------------------------------------------------------*/ 
