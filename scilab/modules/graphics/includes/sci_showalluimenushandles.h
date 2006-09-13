/*------------------------------------------------------------------------*/
/* file: sci_showalluimenushandles.h                                      */
/* Copyright INRIA 2006                                                   */
/* Authors : Allan Cornet, Fabrice Leray, Jean-Baptiste Silvy             */
/* desc : interface for howalluimenushandles routine                      */
/*------------------------------------------------------------------------*/

#ifndef _INT_SHOWALLUIMENUSHANDLES_H_
#define _INT_SHOWALLUIMENUSHANDLES_H_

#include "ObjectStructure.h"

/**
* interface function for the showalluimenushandles routine.
*
* @param[in] fname     name of the routine (ie showalluimenushandles)
* @param[in] fname_len length of fname
* @return 0 if success, -1 otherwise
*/
int sci_showalluimenushandles( char * fname, unsigned long fname_len ) ;

int SciShowAllUimenus( sciPointObj* pparent ) ;

#endif /* _INT_SHOWALLUIMENUSHANDLES_H_ */
