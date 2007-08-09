/*------------------------------------------------------------------------*/
/* file: DrawableArc.cpp                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for an arc object          */
/*------------------------------------------------------------------------*/

#include "DrawableArc.h"

namespace sciGraphics
{


/*------------------------------------------------------------------------------------------*/
DrawableArc::DrawableArc( sciPointObj * pObj ) : DrawableClippedObject(pObj)
{

}
/*------------------------------------------------------------------------------------------*/
DrawableArc::~DrawableArc( void )
{
  //removeDrawingStrategies();
}
/*------------------------------------------------------------------------------------------*/
void DrawableArc::draw( void )
{
  initializeDrawing() ;
  if ( !checkVisibility() )
  {
    endDrawing();
    return ;
  }
  clip();
  drawArc() ;
  unClip();
  endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void DrawableArc::show( void )
{
  clip();
  showArc();
  unClip();
}
/*------------------------------------------------------------------------------------------*/
DrawableArcBridge * DrawableArc::getArcImp( void )
{
  return dynamic_cast<DrawableArcBridge *>(m_pImp);
}
/*------------------------------------------------------------------------------------------*/

}
