/*------------------------------------------------------------------------*/
/* file: DrawableSubwin.cpp                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a subwin object        */
/*------------------------------------------------------------------------*/

#include "DrawableSubwin.h"
#include "DrawableSubwinBridge.h"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableSubwin::DrawableSubwin(sciPointObj * pObj) : DrawableClippedObject(pObj)
{
  m_pCamera = NULL ;
}
/*------------------------------------------------------------------------------------------*/
DrawableSubwin::~DrawableSubwin( void )
{
  if ( m_pCamera != NULL )
  {
    delete m_pCamera ;
  }
  m_pCamera = NULL;
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwin::draw( void )
{
  //initializeDrawing() ;
  //if ( !checkVisibility() )
  //{
  //  endDrawing();
  //  return;
  //}
  m_pCamera->setViewingArea(sciGetAxesBounds(m_pDrawed), sciGetMargins(m_pDrawed)) ;

  double bounds[6] ;
  sciGetRealDataBounds(m_pDrawed, bounds) ;
  m_pCamera->setSubwinBox(bounds) ;
  m_pCamera->renderPosition();
  displayChildren() ;
  //endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwin::show( void )
{
  draw() ;
}
/*------------------------------------------------------------------------------------------*/
DrawableSubwinBridge * DrawableSubwin::getSubwinImp( void )
{
  return dynamic_cast<DrawableSubwinBridge *>(m_pImp) ;
}
/*------------------------------------------------------------------------------------------*/

}
