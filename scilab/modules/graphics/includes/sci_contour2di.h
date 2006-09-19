/*------------------------------------------------------------------------*/
/* file: sci_contour2di.h                                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for contour2di routine                                */
/*------------------------------------------------------------------------*/

#ifndef _INT_CONTOUR2DI_H_
#define _INT_CONTOUR2DI_H_

/**
* interface function for the contour2di routine.
*
* @param[in] fname      name of the routine (ie contour2di)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_contour2di( char * fname, unsigned long fname_len ) ;

#endif /* _INT_CONTOUR2DI_H_ */
