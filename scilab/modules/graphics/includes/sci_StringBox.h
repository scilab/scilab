/*------------------------------------------------------------------------*/
/* file: sci_StringBox.h                                                  */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Jean-Baptiste Silvy                           */
/* desc : interface for StringBox routine                                 */
/*------------------------------------------------------------------------*/

#ifndef _INT_STRINGBOX_H_
#define _INT_STRINGBOX_H_

/**
* interface function for the StringBox routine :
* rect = StringBox( handle )
*
* @param[in] fname     name of the routine (ie StringBox)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_StringBox( char * fname, unsigned long fname_len ) ;

#endif /* _INT_STRINGBOX_H_ */
