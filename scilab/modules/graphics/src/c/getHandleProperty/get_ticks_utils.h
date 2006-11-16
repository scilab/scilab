/*------------------------------------------------------------------------*/
/* file: get_ticks_utils.h                                                */
/* Copyright INRIA 2006                                                   */
/* Authors : Fabrice Leray, Allan Cornet, Jean-Baptiste Silvy             */
/* desc : a set of functions used to return ticks tlist in scilab         */
/*------------------------------------------------------------------------*/

#ifndef _GET_TICKS_UTILS_H_
#define _GET_TICKS_UTILS_H_

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "returnPropertyList.h"
#include "sciprint.h"

int buildTListForTicks( const double * locations, const char * labels[], int nbTics ) ;

#endif /* _GET_TICKS_UTILS_H_ */
