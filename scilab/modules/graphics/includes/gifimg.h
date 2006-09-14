/*------------------------------------------------------------------------*/
/* file: gifimg.h                                                         */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Jean-Baptiste Silvy                            */
/* desc : Routines to handle gif images in scilab                         */
/*------------------------------------------------------------------------*/

#ifndef _GIFIMG_H_
#define _GIFIMG_H_

#include "../src/gd/gd.h"

void C2F(deallocategifimg)( gdImagePtr * im ) ;
void deallocateGifImg( gdImagePtr * im ) ;

void C2F(readgifimg)( char * string, gdImagePtr * imgptr, int * m,int * n, int * ncol) ;
void readGifImg( char * string, gdImagePtr * imgptr, int * m, int * n, int * ncol) ;

void C2F(readxbmimg)( char * string, gdImagePtr * imgptr, int * m, int * n, int * ncol ) ;
void readXbmImg( char * string, gdImagePtr * imgptr, int * m, int * n, int * ncol) ;

void C2F(getgifpixels)( gdImagePtr * imgptr, unsigned char * pixels ) ;
void getGifPixels( gdImagePtr * imgptr, unsigned char * pixels ) ;

void C2F(getgifcmap)( gdImagePtr * imgptr, unsigned char * cmap ) ;
void getGifCmap( gdImagePtr * imgptr, unsigned char * cmap ) ;

#endif /* _GIFIMG_H_ */
