/*------------------------------------------------------------------------*/
/* file: graphicModuleClose.c                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : function to be called each time the graphic module is unloaded  */
/*------------------------------------------------------------------------*/

#include "getHandleProperty/GetHashTable.h"
#include "getHandleProperty/SetHashTable.h"
#include "DestroyObjects.h"
#include "graphicModuleClose.h"
#include "InitObjects.h"
#include "periScreen.h"

/*------------------------------------------------------------------------*/
void closeGraphicModule( void )
{
  /* destroy hashtables */
  destroyScilabGetHashTable() ;
  destroyScilabSetHashTable() ;


  /* destroy all graphic windows */
  AllGraphWinDelete() ;

  /* destroy default objects */
  destroyDefaultObjects() ;

  /* deleteTemporary points points in peri***.c */
  deletePoints() ;
}
/*------------------------------------------------------------------------*/
