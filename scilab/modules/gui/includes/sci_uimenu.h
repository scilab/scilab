/*------------------------------------------------------------------------*/
/* file: sci_uimenu.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Fabrice Leray, Jean-Baptiste Silvy             */
/* desc : interface for sci_uimenu routine                                */
/*------------------------------------------------------------------------*/

#ifndef _INT_UIMENU_H_
#define _INT_UIMENU_H_

/**
* interface function for the uimenu routine.
*
* @param[in] fname     name of the routine (ie uimenu)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_uimenu( char * fname, unsigned long fname_len ) ;

#endif /* _INT_UIMENU_H_ */
