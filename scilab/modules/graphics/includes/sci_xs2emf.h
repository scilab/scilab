/*------------------------------------------------------------------------*/
/* file: sci_xs2emf.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xs2emf routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XS2EMF_H_
#define _INT_XS2EMF_H_

/**
 * interface function for the xs2emf routine :
 *
 * @param[in] fname     name of the routine (ie xs2emf)
 * @param[in] fname_len length of fname
 * @return 0 if success, -1 otherwise
 */
int sci_xs2emf( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XS2EMF_H_ */
