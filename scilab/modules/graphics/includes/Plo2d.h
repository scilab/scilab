/*------------------------------------------------------------------------
*    Graphic library
*    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
*    jpc@cermics.enpc.fr 
*    Copyright INRIA 2006
*    Jean-Baptiste Silvy
--------------------------------------------------------------------------*/

#include "machine.h"

int C2F(plot2d)(double    x[]      ,
                double    y[]      ,
                integer * n1       ,
                integer * n2       ,
                integer   style[]  ,
                char      strflag[],
                char      legend[] ,
                double    brect[]  ,
                integer   aaint[]  ,
                integer   lstr1    ,
                integer   lstr2     ) ;

int C2F(xgrid)( integer * style ) ;

void Legends( integer * style, integer * n1, char * legend ) ;
