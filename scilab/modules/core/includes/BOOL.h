/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#ifndef __BOOL_H__
#define __BOOL_H__

typedef int integer;

/* define boolean type */
#ifdef BOOL
	#undef BOOL
#endif

typedef int BOOL ;

#ifdef TRUE
	#undef TRUE
#endif
#define TRUE 1

#ifdef FALSE
	#undef FALSE
#endif
#define FALSE 0

#endif /* __BOOL_H__ */
/*--------------------------------------------------------------------------*/
