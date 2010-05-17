/*-----------------------------------------------------------------------------------*/ 
/* Allan CORNET */
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCILAB_MALLOC_H__
#define __SCILAB_MALLOC_H__

#ifdef __cplusplus
	extern "C"
	{
#endif

	#ifdef _MSC_VER
		#include "win_mem_alloc.h" /* MALLOC */	
	#else /* others O.S*/
		#include "sci_mem_alloc.h" /* MALLOC */
	#endif

#ifdef __cplusplus
	}; 
#endif /* extern "C" */

#endif /* __SCILAB_MALLOC_H__ */
/*-----------------------------------------------------------------------------------*/ 
