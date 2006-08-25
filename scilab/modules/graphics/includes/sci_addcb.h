/*------------------------------------------------------------------------*/
/* file: sci_addcb.h                                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy, Jean-Philipe Chancelier  */
/* desc : interface for addcb routine                                     */
/*------------------------------------------------------------------------*/

#ifndef _INT_ADDCB_H_
#define _INT_ADDCB_H_

/**
* interface function for the addcb routine (addcb means "add callback").
*
* @param[in] fname     name of the routine (ie addcb)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_addcb( char * fname, unsigned long fname_len ) ;

#endif /* _INT_ADDCB_H_ */
