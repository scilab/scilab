/*-----------------------------------------------------------------------------------*/
/* Copyright (C) 1998-2000 INRIA/Enpc */
/* Fabrice LERAY INRIA 2005 */
/* Allan CORNET INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#ifndef  __INTCOMMONGAPHICS__
#define   __INTCOMMONGAPHICS__
/*-----------------------------------------------------------------------------------*/
#include "../stack-c.h"
/*-----------------------------------------------------------------------------------*/
#if WIN32
#if _MSC_VER <=1200
#define hstk(x) ((C2F(stack).Stk) + x-1 ) 	
#else
#define hstk(x) (((long long *) C2F(stack).Stk) + x-1 )
#endif
#else
#define hstk(x) (((long long *) C2F(stack).Stk) + x-1 )
#endif


extern char *pmodes[];
extern int C2F(sciwin)  _PARAMS((void));
#define SciWin() if(C2F(sciwin)() !=0) { Scierror(999,"%s :Requested figure cannot be created \r\n",fname);return 0;  }



#endif /*   __INTCOMMONGAPHICS__ */
/*-----------------------------------------------------------------------------------*/
