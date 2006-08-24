/*------------------------------------------------------------------------*/
/* file: sci_unzoom.h                                                  */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
/* desc : interface for unzoom routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_UNZOOM_H_
#define _INT_UNZOOM_H_

/**
* interface function for the unzoom routine.
*
* @param[in] fname     name of the routine (ie unzoom)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_unzoom( char * fname, unsigned long fname_len ) ;

#endif /* _INT_UNZOOM_H_ */
