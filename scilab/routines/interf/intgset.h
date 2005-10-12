/*-----------------------------------------------------------------------------------*/
/* Copyright (C) 1998-2000 INRIA/Enpc */
/* Fabrice LERAY INRIA 2005 */
/* Allan CORNET INRIA 2005 */
/*-----------------------------------------------------------------------------------*/
#ifndef __INTGSET__
#define  __INTGSET__

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "../graphics/ObjectStructure.h"


int gset(char *fname,unsigned long fname_len);
int CheckAndUpdate_x_shift(sciPointObj * pobj, int numrow);
int CheckAndUpdate_y_shift(sciPointObj * pobj, int numrow);
int CheckAndUpdate_z_shift(sciPointObj * pobj, int numrow);

#endif /*__INTGSET__*/
/*-----------------------------------------------------------------------------------*/
