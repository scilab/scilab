/*-----------------------------------------------------------------------------------*/
/* Copyright (C) 1998-2000 INRIA/Enpc */
/* Fabrice LERAY INRIA 2005 */
/* Allan CORNET INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#ifndef  __INTCOMMONGRAPHICS__
#define   __INTCOMMONGRAPHICS__
/*-----------------------------------------------------------------------------------*/
#include "../stack-c.h"
#include "matdes.h"
/*-----------------------------------------------------------------------------------*/
extern char *pmodes[];
extern int C2F(sciwin)  _PARAMS((void));
#define SciWin() if(C2F(sciwin)() !=0) { Scierror(999,"%s :Requested figure cannot be created \r\n",fname);return 0;  }

#endif /*   __INTCOMMONGRAPHICS__ */
/*-----------------------------------------------------------------------------------*/
