/*------------------------------------------------------------------------*/
/* file: Fec.h                                                            */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Functions used to draw fec objects                              */
/*------------------------------------------------------------------------*/

#ifndef _FEC_H_
#define _FEC_H_

void newfec(integer *xm,integer *ym,double *triangles,double *func,integer *Nnode,
            integer *Ntr,double *zminmax,integer *colminmax, integer *colout, BOOL with_mesh) ;

#endif /* _FEC_H_ */
