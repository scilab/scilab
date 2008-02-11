/*------------------------------------------------------------------------*/
/* file: sci_mpopup.h                                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Vincent COUVERT                                              */
/* desc : interface for sci_mpopup routine                                */
/*       (temporary function waiting for uicontextmenu function)          */
/*------------------------------------------------------------------------*/

#ifndef __MPOPUP_H__
#define __MPOPUP_H__

/**
* interface function for the mpopup routine.
*
* @param[in] fname name of the routine (ie mpopup)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_mpopup( char * fname, unsigned long fname_len ) ;

#endif /* !__MPOPUP_H__ */
