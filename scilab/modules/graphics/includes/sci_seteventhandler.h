/*------------------------------------------------------------------------*/
/* file: sci_seteventhandler.h                                            */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
/* desc : interface for seteventhandler routine                           */
/*------------------------------------------------------------------------*/

#ifndef _INT_SETEVENTHANDLER_H_
#define _INT_SETEVENTHANDLER_H_

/**
* interface function for the seteventhandler routine.
*
* @param[in] fname     name of the routine (ie seteventhandler)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_seteventhandler( char * fname, unsigned long fname_len ) ;

#endif /* _INT_SETEVENTHANLDER_H_ */
