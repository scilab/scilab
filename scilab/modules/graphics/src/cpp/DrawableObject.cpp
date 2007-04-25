/*------------------------------------------------------------------------*/
/* file: DrawableObject.cpp                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class dedicated for the graphic rendering of graphic handles    */
/*------------------------------------------------------------------------*/

extern "C"
{
#include "GetProperty.h"
}

#include "DrawableObject.h"
#include "getHandleDrawer.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
void DrawableObject::drawChildren( void )
{
  sciSons * curSon = sciGetLastSons( m_pDrawed ) ;
  while ( curSon != NULL )
  {
    getHandleDrawer( curSon->pointobj )->draw() ;
    curSon = curSon->pprev ;
  }
}
/*------------------------------------------------------------------------------------------*/
bool DrawableObject::checkVisibility( void )
{
  return sciGetVisibility( m_pDrawed ) == TRUE ;
}
/*------------------------------------------------------------------------------------------*/

}
