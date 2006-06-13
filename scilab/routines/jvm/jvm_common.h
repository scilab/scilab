/*-----------------------------------------------------------------------------------*/ 
/* INRIA 2006 */
/* Allan CORNET*/
/*-----------------------------------------------------------------------------------*/ 
#define JRE_PATH "/Java/jre1.5.0_06"
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
	#ifdef EXPORT_LIBJVM_DLL
		#define IMPORT_EXPORT_LIBJVM_DLL __declspec(dllexport)
	#else
		#define IMPORT_EXPORT_LIBJVM_DLL __declspec(dllimport)
	#endif
#else
	#define IMPORT_EXPORT_LIBJVM_DLL   extern
#endif

#ifdef _MSC_VER
	#define IMPORT_DLL __declspec(dllimport)
#else
	#define IMPORT_DLL   extern
#endif
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
	#define PATH_SEPARATOR ';'
#else /* UNIX */
	#define PATH_SEPARATOR ':'
#endif

#define USER_CLASSPATH "."
/*-----------------------------------------------------------------------------------*/ 
