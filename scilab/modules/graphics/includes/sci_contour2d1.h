/*------------------------------------------------------------------------*/
/* file: sci_param3d.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for contour2d1 routine                                */
/*------------------------------------------------------------------------*/

#ifndef _INT_CONTOUR2D1_H_
#define _INT_CONTOUR2D1_H_

/**
* interface function for the contour2d1 routine :
* @param[in] fname      name of the routine (ie param3d)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_contour2d1( char * fname, unsigned long fname_len ) ;

#endif