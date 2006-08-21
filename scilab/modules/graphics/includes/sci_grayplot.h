/*------------------------------------------------------------------------*/
/* file: sci_grayplot.h                                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for grayplot routine                                  */
/*------------------------------------------------------------------------*/

#ifndef _INT_GRAYPLOT_H_
#define _INT_GRAYPLOT_H_

/**
* interface function for the grayplot routine :
*  grayplot(x,y,z,[strf,rect,nax])
*
* @param[in] fname      name of the routine (ie grayplot)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_grayplot( char * fname, unsigned long fname_len ) ;

#endif /* _INT_GEOM3D_H_ */
