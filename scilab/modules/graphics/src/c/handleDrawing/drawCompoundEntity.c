/*------------------------------------------------------------------------*/
/* file: drawCompoundEntity.c                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a compound handle             */
/*------------------------------------------------------------------------*/

#include "drawCompoundEntity.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "DrawingBridge.h"

/*-----------------------------------------------------------------------------------*/
int drawCompoundEntity( sciPointObj * pObj )
{
  sciSons * curSon = NULL ;
  
  if ( !sciGetVisibility(pObj) ) { return 0 ; }

  /* scan the hierarchy and call sciDrawObj */
  curSon = sciGetLastSons( pObj ) ;
  while ( curSon != NULL )
  {
    sciDrawObj( curSon->pointobj ) ;
    curSon = curSon->pprev ;
  }

  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
