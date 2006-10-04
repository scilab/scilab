/*------------------------------------------------------------------------*/
/* file: graphicModuleClose.c                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : function to be called each time the graphic module is unloaded  */
/*------------------------------------------------------------------------*/

#include "getHandleProperty/GetHashTable.h"
#include "getHandleProperty/SetHashTable.h"
#include "DestroyObjects.h"

/*------------------------------------------------------------------------*/
void closeGraphicModule( void )
{
  /* destroy hashtables */
  destroyScilabGetHashTable() ;
  destroyScilabSetHashTable() ;


  /* destroy all graphic windows */
  AllGraphWinDelete() ;
}
/*------------------------------------------------------------------------*/
