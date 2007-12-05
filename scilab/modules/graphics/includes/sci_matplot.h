/*------------------------------------------------------------------------*/
/* file: sci_matplot.h                                                    */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for matplot routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_MATPLOT_H_
#define _INT_MATPLOT_H_

/**
* interface function for the matplot routine :
*
* @param[in] fname      name of the routine (ie matplot)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_matlot( char * fname, unsigned long fname_len ) ;

/**
 * @TODO add comment
 *
 * @param fname 
 * @param fname_len 
 * @return <ReturnValue>
 */
int sci_matplot(char *fname,unsigned long fname_len);

#endif /* _INT_MATPLOT_H_ */
