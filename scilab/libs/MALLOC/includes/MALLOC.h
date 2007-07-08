/*-----------------------------------------------------------------------------------*/ 
/* Allan CORNET */
/* INRIA 2006 */
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
	#ifdef __cplusplus
	extern "C"
	{
		#include "win_mem_alloc.h" /* MALLOC */
	};
	#else
		#include "win_mem_alloc.h" /* MALLOC */
	#endif
#else
	#ifdef __cplusplus
	extern "C"
	{
		#include "sci_mem_alloc.h" /* MALLOC */
	}
	#else
		#include "sci_mem_alloc.h" /* MALLOC */
	#endif
#endif
/*-----------------------------------------------------------------------------------*/ 
