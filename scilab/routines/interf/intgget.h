/*-----------------------------------------------------------------------------------*/
/* Copyright (C) 1998-2000 INRIA/Enpc */
/* Fabrice LERAY INRIA 2005 */
/* Allan CORNET INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#ifndef __INTGGET__
#define  __INTGGET__

#include <string.h>
#include <stdio.h>
#include <math.h>

int gget(char *fname,unsigned long fname_len);

/*-----------------------------------------------------------------------------------*/
/* copy the string value into the scilab stack for output */
int sciReturnString( const char * value ) ;

/* copy an int in the scilab stack for output */
int sciReturnInt( int value ) ;

/*-----------------------------------------------------------------------------------*/

#endif /*__INTGGET__*/
/*-----------------------------------------------------------------------------------*/
