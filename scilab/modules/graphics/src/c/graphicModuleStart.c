/*------------------------------------------------------------------------*/
/* file: graphicModuleStart.c                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : function to be called each time the graphic module is loaded    */
/*------------------------------------------------------------------------*/

#include "getHandleProperty/GetHashTable.h"
#include "getHandleProperty/SetHashTable.h"
#include "graphicModuleStart.h"

/*------------------------------------------------------------------------*/
void startGraphicModule( void )
{
  createScilabGetHashTable() ;
  createScilabSetHashTable() ;
}
/*------------------------------------------------------------------------*/
