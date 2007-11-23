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

/*---------------------------------------------------------------------------------*/
DrawableSubwin::DrawableSubwin(sciPointObj * pObj) : DrawableObject(pObj)
{
  m_pCamera = NULL ;
}
/*---------------------------------------------------------------------------------*/
DrawableSubwin::~DrawableSubwin( void )
{
  if ( m_pCamera != NULL )
  {
    delete m_pCamera ;
    m_pCamera = NULL ;
  }
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::setCamera( Camera * cam )
{
  if (m_pCamera != NULL)
  {
    delete m_pCamera;
    m_pCamera = NULL;
  }
  m_pCamera = cam;
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::draw( void )
{
  m_pCamera->hasChanged();
  show();
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::show( void )
{
  initializeDrawing() ;
  if ( !checkVisibility() )
  {
    endDrawing();
    return;
  }

  // set up camera
  m_pCamera->display();

  double bounds[6] ;
  sciGetRealDataBounds(m_pDrawed, bounds) ;
  drawBox(bounds);

  displayChildren() ;

  // needed
  m_pCamera->replaceCamera();

  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void DrawableSubwin::drawBox(const double bounds[6])
{
  getSubwinImp()->drawBox(bounds);
}
/*---------------------------------------------------------------------------------*/
DrawableSubwinBridge * DrawableSubwin::getSubwinImp( void )
{
  return dynamic_cast<DrawableSubwinBridge *>(m_pImp) ;
}
/*---------------------------------------------------------------------------------*/

}
