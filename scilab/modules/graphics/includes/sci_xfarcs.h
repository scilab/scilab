/*------------------------------------------------------------------------*/
/* file: sci_xfarcs.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xfarcs routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XFARCS_H_
#define _INT_XFARCS_H_

/**
* interface function for the xfarcs routine :
* xfarcs(arcs,[style])
*
* @param[in] fname     name of the routine (ie xfarcs)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xfarcs( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XFARCS_H_ */
