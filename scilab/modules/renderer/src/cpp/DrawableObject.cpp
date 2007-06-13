/*------------------------------------------------------------------------*/
/* file: DrawableObject.cpp                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class dedicated for the graphic rendering of graphic handles    */
/*------------------------------------------------------------------------*/

extern "C"
{
#include "../../../graphics/includes/GetProperty.h"
}

#include "DrawableObject.h"
#include "getHandleDrawer.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject::DrawableObject( sciPointObj * drawed )
{
  m_pDrawed     = drawed ;
  m_bNeedRedraw = true   ; // a first call to draw is necessary
  m_pImp = NULL ;
}
/*------------------------------------------------------------------------------------------*/
DrawableObject::~DrawableObject( void )
{
  if ( m_pImp != NULL )
  {
    delete m_pImp ;
    m_pImp = NULL ;
  }
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::display( void )
{
  if ( m_bNeedRedraw )
  {
    draw() ;
    m_bNeedRedraw = false ;
  }
  show() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::hasChanged( void )
{
  m_bNeedRedraw = true ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::familyHasChanged( void )
{
  hasChanged();
  sciSons * curSon = sciGetLastSons( m_pDrawed ) ;
  while ( curSon != NULL )
  {
    getHandleDrawer( curSon->pointobj )->familyHasChanged();
    curSon = curSon->pprev ;
  }
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::displayChildren( void )
{
  sciSons * curSon = sciGetLastSons( m_pDrawed ) ;
  while ( curSon != NULL )
  {
    getHandleDrawer( curSon->pointobj )->display() ;
    curSon = curSon->pprev ;
  }
}
/*------------------------------------------------------------------------------------------*/
bool DrawableObject::checkVisibility( void )
{
  return sciGetVisibility( m_pDrawed ) == TRUE ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::initializeDrawing( void )
{
  getDrawableImp()->initializeDrawing() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableObject::endDrawing( void )
{
  getDrawableImp()->endDrawing() ;
}
/*------------------------------------------------------------------------------------------*/
}
