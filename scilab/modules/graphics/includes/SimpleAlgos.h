/*------------------------------------------------------------------------*/
/* file: SimpleAlgos.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : Several basic algorithm which can be used in several places in  */
/*        the code.                                                       */
/*------------------------------------------------------------------------*/

#ifndef _SIMPLE_ALGOS_H_
#define _SIMPLE_ALGOS_H_

/**
 * Find the striclty positive minimum value of an array of double.
 * @param[in] x array of size n.
 * @return The stricly posistive minimum of x if exists, a negative value otherwise
 */
double sciFindStPosMin( double x[], int n ) ;

#endif /* _SIMPLE_ALGOS_H_ */