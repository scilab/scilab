/*------------------------------------------------------------------------*/
/* file: sci_fec.h                                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for sci_fec routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _INT_FEC_H_
#define _INT_FEC_H_

/**
* interface function for the fec routine :
* fec(x,y,triangles,func,[strf,leg,rect,nax,zminmax,colminmax]);
*
* @param[in] fname     name of the routine (ie fec)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_fec( char * fname, unsigned long fname_len ) ;

#endif /* _INT_FEC_H_ */
