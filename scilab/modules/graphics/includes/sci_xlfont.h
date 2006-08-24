/*------------------------------------------------------------------------*/
/* file: sci_xlfont.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for xlfont routine                                    */
/*------------------------------------------------------------------------*/

#ifndef _INT_XLFONT_H_
#define _INT_XLFONT_H_

/**
* interface function for the xlfont routine :
* xlfont(font-name,font-id)
* fonts=xlfont()
* Warning sz dimensions must be compatible with periX11.c FONTNUMBER
*
* @param[in] fname     name of the routine (ie xlfont)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_xlfont( char * fname, unsigned long fname_len ) ;

#endif /* _INT_XLFONT_H_ */
