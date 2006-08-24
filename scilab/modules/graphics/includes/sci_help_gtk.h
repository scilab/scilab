/*------------------------------------------------------------------------*/
/* file: sci_help_gtk.h                                                   */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for help_gtk routine                                  */
/*------------------------------------------------------------------------*/

#ifndef _INT_HELP_GTK_H_
#define _INT_HELP_GTK_H_

/**
* interface function for the help_gtk routine.
*
* @param[in] fname     name of the routine (ie help_gtk)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*
* To be moved to a trash module or deleted.
*/
int sci_help_gtk( char * fname, unsigned long fname_len ) ;

#endif /* _INT_HELP_GTK_H_ */
