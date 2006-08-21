/*------------------------------------------------------------------------*/
/* file: sci_xsegs.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xsegs routine                                     */
/*------------------------------------------------------------------------*/

#ifndef _INT_XSEGS_H_
#define _INT_XSEGS_H_

/**
* interface function for the xsegs routine :
* xsegs(xv,yv,[style])
*
* @param[in] fname     name of the routine (ie xsegs)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xsegs( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XSEGS_H_ */
