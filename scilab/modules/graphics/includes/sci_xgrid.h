/*------------------------------------------------------------------------*/
/* file: sci_grid.h                                                       */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xgrid routine                                      */
/*------------------------------------------------------------------------*/

#ifndef _INT_XGRID_H_
#define _INT_XGRID_H_

/**
 * interface function for the xgrid routine :
 * xgrid([style])
 *
 * @param[in] fname     name of the routine (ie xgrid)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
 */
int sci_xgrid( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XGRID_H_ */
