/*------------------------------------------------------------------------*/
/* file: graphicModuleClose.c                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : function to be called each time the graphic module is unloaded  */
/*------------------------------------------------------------------------*/

#include "getHandleProperty/GetHashTable.h"
#include "getHandleProperty/SetHashTable.h"
#include "DestroyObjects.h"
#include "graphicModuleLoad.h"
#include "InitObjects.h"
#include "periScreen.h"
#include "getScilabJavaVM.h"
#include "JniUtils.h"

static BOOL isGraphicModuleLoaded = FALSE ;

/*------------------------------------------------------------------------*/
void loadGraphicModule( void )
{
  if ( isGraphicModuleLoaded ) { return ; }
  
  createScilabGetHashTable() ;
  createScilabSetHashTable() ;

  C2F(graphicsmodels)() ;

  jniInitUtils( getScilabJavaVM() ) ;

  isGraphicModuleLoaded = TRUE ;
}
/*------------------------------------------------------------------------*/
void closeGraphicModule( void )
{
  if ( !isGraphicModuleLoaded ) { return ; }

  /* destroy hashtables */
  destroyScilabGetHashTable() ;
  destroyScilabSetHashTable() ;


  /* destroy all graphic windows */
  AllGraphWinDelete() ;

  /* destroy default objects */
  destroyDefaultObjects() ;

  /* deleteTemporary points points in peri***.c */
  deletePoints() ;

  jniCloseUtils() ;

  isGraphicModuleLoaded = FALSE ;

}
/*------------------------------------------------------------------------*/
