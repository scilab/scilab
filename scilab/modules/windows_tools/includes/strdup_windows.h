/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifndef __STRDUP_WIN_H__
#define __STRDUP_WIN_H__

/* strdup is deprecated on Windows with VS 2005 and more */
/* required to fix warnings about strdup */

#ifdef _MSC_VER
	#ifdef strdup
	#undef strdup
	#endif
	#define strdup strdup_windows

	/**
	* Duplicate strings
	* @param[in] string source
	* @return a pointer to the storage location for the copied string 
	*/
	char *strdup_windows(const char *strSource);
#endif

#endif /* __STRDUP_WIN_H__ */
/*--------------------------------------------------------------------------*/