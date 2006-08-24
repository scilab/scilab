/*------------------------------------------------------------------------*/
/* file: BAsicAlgos.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : Several basic algorithm which can be used in several places in  */
/*        the code.                                                       */
/*------------------------------------------------------------------------*/

#ifndef _BASIC_ALGOS_H_
#define _BASIC_ALGOS_H_

#include "machine.h"

/**
 * Find the striclty positive minimum value of an array of double.
 * @param[in] x array of size n.
 * @return The stricly posistive minimum of x if exists, a negative value otherwise
 */
double sciFindStPosMin( double x[], int n ) ;

/**
 *     convertion d'entier vers double
 *     d et s peuvent en fait pointer sur le meme tableau
 *     car la recopie est fait de n,1,-1
 *      implicit undefined (a-z)
 */
int C2F(entier2d)( integer * n, double * dx,integer * s ) ;

#endif /* _BASIC_ALGOS_H_ */