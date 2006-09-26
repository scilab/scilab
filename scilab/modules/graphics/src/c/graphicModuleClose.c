/*------------------------------------------------------------------------*/
/* file: graphicModuleClose.c                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : function to be called each time the graphic module is unloaded  */
/*------------------------------------------------------------------------*/

#include "getHandleProperty/GetHashTable.h"
#include "DestroyObjects.h"

/*------------------------------------------------------------------------*/
void closeGraphicModule( void )
{
  /* destroy get hashtable */
  destroyScilabHashTable() ;

  /* destroy all graphic windows */
  AllGraphWinDelete() ;
}
/*------------------------------------------------------------------------*/
