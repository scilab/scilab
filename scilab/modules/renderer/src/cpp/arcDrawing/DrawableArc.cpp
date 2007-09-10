/*------------------------------------------------------------------------*/
/* file: DrawableArc.cpp                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for an arc object          */
/*------------------------------------------------------------------------*/

#include "DrawableArc.h"
#include <iostream>

namespace sciGraphics
{


/*------------------------------------------------------------------------------------------*/
DrawableArc::DrawableArc( sciPointObj * pObj ) : DrawableClippedObject(pObj)
{
  
}
/*------------------------------------------------------------------------------------------*/
DrawableArc::~DrawableArc( void )
{
  
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
