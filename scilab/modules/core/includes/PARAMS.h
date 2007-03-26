/* INRIA */

#ifndef __PARAMS_H__
#define __PARAMS_H__

#if defined(__STDC__) || defined(_MSC_VER)
	#ifndef  __PARAMS
		#define  __PARAMS(paramlist)		paramlist
	#endif

	#ifndef  _PARAMS
		#define  _PARAMS(paramlist)		paramlist
	#endif
#else
	#ifndef  __PARAMS
		#define  __PARAMS(paramlist)		()
	#endif
	
	#ifndef  _PARAMS
		#define  _PARAMS(paramlist)		()
	#endif
#endif

#endif /* __PARAMS_H__ */

