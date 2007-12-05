/*------------------------------------------------------------------------*/
/* file: sci_matplot1.h                                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for matplot1 routine                                  */
/*------------------------------------------------------------------------*/

#ifndef _INT_MATPLOT1_H_
#define _INT_MATPLOT1_H_

/**
* interface function for the matplot1 routine :
*
* @param[in] fname      name of the routine (ie matplot1)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_matlot1( char * fname, unsigned long fname_len ) ;

/**
 * @TODO add comment
 *
 * @param fname 
 * @param fname_len 
 * @return <ReturnValue>
 */
int sci_matplot1( char * fname, unsigned long fname_len );

#endif /* _INT_MATPLOT1_H_ */
