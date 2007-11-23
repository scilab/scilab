/*------------------------------------------------------------------------*/
/* file: sci_set.h                                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : interface for sci_set routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_SET_H_
#define _INT_SET_H_

#include "ObjectStructure.h"


int sci_set(char *fname,unsigned long fname_len);

/*--------------------------------------------------------------------------*/

int CheckAndUpdate_x_shift( sciPointObj * pobj, int numrow );
int CheckAndUpdate_y_shift( sciPointObj * pobj, int numrow );
int CheckAndUpdate_z_shift( sciPointObj * pobj, int numrow );

/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/

#endif /*_INT_SET_H_*/
/*--------------------------------------------------------------------------*/
