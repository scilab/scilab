/*------------------------------------------------------------------------*/
/* file: CheckTicksProperty.h                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : generic function to retrieve the assigned x_ticks, y_ticks or   */
/*        z_ticks property                                                */
/*------------------------------------------------------------------------*/

#ifndef _CHECK_TICKS_PROPERTY_C_
#define _CHECK_TICKS_PROPERTY_C_


#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"

/**
 * create a new assigne dlist which allow to easily retrieve the ticks
 * properties from the stack. Moreover the functgion checked if the list
 * contains the right properties.
 * @return the newly created list or NULL if an error occured.
 */
AssignedList * createTlistForTicks( void ) ;

#endif /* _CHECK_TICKS_PROPERTY_C_ */
