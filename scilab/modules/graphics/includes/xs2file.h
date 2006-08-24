/*------------------------------------------------------------------------*/
/* file: xs2file.h                                                        */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xs2file routine                                   */
/*------------------------------------------------------------------------*/

#ifndef _XS2FILE_H_
#define _XS2FILE_H_

#include "machine.h"

/**
* Generic function to create either ps, fig, gif or ppm files.
*
* @param[in] fname     name of the routine (ie xs2ps, xs2fig,...)
* @param[in] fname_len length of fname
* @param[in] dr        specify the file type. Must be oen of the following
*                      driver names "Pos", "Fig", "GIF" or "PPM".
* @param[in@ dr_len    length of the dr string (without \0 apparently.
* @return 0 if success, -1 otherwise
*/
int xs2file( char *fname,char *dr, unsigned long fname_len, unsigned long dr_len ) ;

/**
* send recorded graphics to file bufname in ``driver'' syntax ( Pos or Fig )
* @param win_num the number of the window,
* @param colored 1 if color is wanted
* @param[in] bufname string the name of the file 
* @param[in] driver driver for code generation 
*/
int scig_toPs( integer win_num, integer colored, char * bufname, char * driver ) ;

int C2F(xg2psofig)(char *fname, integer *len, integer *iwin, integer *color, char *driver, long int l1, long int l2) ;

#endif /* _INT_XS2FILE_H_ */
