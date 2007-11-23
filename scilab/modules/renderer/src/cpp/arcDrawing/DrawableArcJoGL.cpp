/*------------------------------------------------------------------------*/
/* file: DrawableArcJoGL.cpp                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw an       */
/*        arc object with JoGL                                            */
/*------------------------------------------------------------------------*/

#include "DrawableArcJoGL.hxx"

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableArcJoGL::DrawableArcJoGL(DrawableArc * arc)
  : DrawableClippedObjectJoGL(arc) 
{

}
/*---------------------------------------------------------------------------------*/
DrawableArcJoGL::~DrawableArcJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
DrawableArc * DrawableArcJoGL::getArcDrawer( void )
{
  return dynamic_cast<DrawableArc *>(getDrawer());
}
/*---------------------------------------------------------------------------------*/
}
