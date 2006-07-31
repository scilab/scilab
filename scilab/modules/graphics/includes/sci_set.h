/*-----------------------------------------------------------------------------------*/
/* Copyright (C) 1998-2000 INRIA/Enpc */
/* Fabrice LERAY INRIA 2005 */
/* Allan CORNET INRIA 2005 */
/* Jean-Baptiste SILVY 2006 */
/*-----------------------------------------------------------------------------------*/
#ifndef __INTGSET__
#define  __INTGSET__

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "ObjectStructure.h"


int gset(char *fname,unsigned long fname_len);
int CheckAndUpdate_x_shift(sciPointObj * pobj, int numrow);
int CheckAndUpdate_y_shift(sciPointObj * pobj, int numrow);
int CheckAndUpdate_z_shift(sciPointObj * pobj, int numrow);

/*-----------------------------------------------------------------------------------*/
/* this function compute the final staus of modifications when two consecutive call */
/* to sciSet*** have been made. A bit tricky I agree. JB Silvy 03/2006 */
int sciSetFinalStatus( int status1, int status2 ) ;

/*-----------------------------------------------------------------------------------*/

#endif /*__INTGSET__*/
/*-----------------------------------------------------------------------------------*/
