/*------------------------------------------------------------------------*/
/* file: getHandleProperty.h                                              */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : contains definitions of functions needed by sci_get             */
/*        Each function has the same signature int (sciPointObj *) and    */
/*        return a value in the scilab console                            */
/*------------------------------------------------------------------------*/

#ifndef _GET_HANDLE_PROPERTY_H_
#define _GET_HANDLE_PROPERTY_H_

#include "ObjectStructure.h"

int get_figures_id_property( sciPointObj * pObj ) ;

#endif /* _GET_HANDLE_PROPERTY_H_ */
