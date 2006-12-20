/*------------------------------------------------------------------------*/
/* file: drawCompoundEntity.c                                             */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains functions needed to draw a compound handle             */
/*------------------------------------------------------------------------*/

#include "drawCompoundEntity.h"
#include "GetProperty.h"
#include "DrawObjects.h"

/*-----------------------------------------------------------------------------------*/
int drawCompoundEntity( sciPointObj * pObj )
{
  sciSons * curSon = NULL ;
  BOOL isMerging = pSUBWIN_FEATURE(sciGetParentSubwin(pObj))->facetmerge ;
  
  if ( !sciGetVisibility(pObj) ) { return 0 ; }

  /* scan the hierarchy and call sciDrawObj */
  curSon = sciGetLastSons( pObj ) ;
  while ( curSon != NULL )
  {
    /* draw only objects which are not already included in a merge objects */
    if ( !isMerging || !sciIsMergeable(curSon->pointobj) )
    {
      sciDrawObj( curSon->pointobj ) ;
    }
    curSon = curSon->pprev ;
  }

  return 0 ;
}
/*-----------------------------------------------------------------------------------*/
