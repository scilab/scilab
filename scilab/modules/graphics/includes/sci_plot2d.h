/*------------------------------------------------------------------------*/
/* file: sci_plot2d.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for plot2d routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_PLOT2D_
#define _INT_PLOT2D_

/**
 * interface function fot the plot2d routine :
 * plot2d(x,y,[style,strf,leg,rect,nax])
 *
 * @param[in] fname      name of the routine (ie plot2d)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
 */
int sci_plot2d( char * fname, unsigned long fname_len ) ;

#endif /* _INT_PLOT2D_ */
