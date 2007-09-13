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
DrawableSubwin::DrawableSubwin(sciPointObj * pObj) : DrawableObject(pObj)
{
  m_pCamera = NULL ;
}
/*------------------------------------------------------------------------------------------*/
DrawableSubwin::~DrawableSubwin( void )
{
  if ( m_pCamera != NULL )
  {
    delete m_pCamera ;
    m_pCamera = NULL ;
  }
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwin::draw( void )
{
  initializeDrawing() ;
  if ( !checkVisibility() )
  {
    endDrawing();
    return;
  }
  m_pCamera->setViewingArea(sciGetAxesBounds(m_pDrawed), sciGetMargins(m_pDrawed)) ;

  double bounds[6] ;
  sciGetRealDataBounds(m_pDrawed, bounds) ;
  m_pCamera->setSubwinBox(bounds) ;

  double alpha;
  double theta;
  sciGetViewingAngles(m_pDrawed, &alpha, &theta);
  m_pCamera->setRotationAngles(alpha, theta);
  m_pCamera->renderPosition();

  drawBox(bounds);

  displayChildren() ;
  endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwin::show( void )
{
  draw() ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableSubwin::drawBox(const double bounds[6])
{
  getSubwinImp()->drawBox(bounds);
}
/*------------------------------------------------------------------------------------------*/
DrawableSubwinBridge * DrawableSubwin::getSubwinImp( void )
{
  return dynamic_cast<DrawableSubwinBridge *>(m_pImp) ;
}
/*------------------------------------------------------------------------------------------*/

}
