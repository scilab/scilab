/*------------------------------------------------------------------------*/
/* file: sci_contour.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for contour routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_CONTOUR_H_
#define _INT_CONTOUR_H_

/**
* interface function fot the contour routine :
* contour(x,y,z,nz,[theta,alpha,leg,flag,ebox,zlev])
*
* @param[in] fname      name of the routine (ie contour)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_contour( char * fname, unsigned long fname_len ) ;

#endif /* _INT_CONTOUR_H_ */