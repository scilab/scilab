/*------------------------------------------------------------------------*/
/* file: sci_plot3d.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for plot3d and plot3d1 routines                       */
/*------------------------------------------------------------------------*/

#ifndef _INT_PLOT3D_H_
#define _INT_PLOT3D_H_

/**
* interface function for the plot3d routine.
*
* @param[in] fname     name of the routine (ie plot3d or plot3d1)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_plot3d( char * fname, unsigned long fname_len ) ;

#endif /* _INT_PLOT3D_H_ */
