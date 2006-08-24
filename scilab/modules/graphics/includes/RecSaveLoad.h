/*------------------------------------------------------------------------
*    Graphic library
*    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
*    jpc@cermics.enpc.fr 
* Copyright INRIA 2006
* Jean-Baptiste Silvy
* save/load graphic code in a file  
* --------------------------------------------------------------------------*/

#ifndef _REC_SAVELOAD_H_
#define _REC_SAVELOAD_H_

#include "machine.h"

int C2F(xsaveplots)( integer * winnumber, char * fname1, integer lxv ) ;

int C2F(xloadplots)( char * fname1, integer lvx) ;

#endif /* _REC_SAVELOAD_H_ */
