/*------------------------------------------------------------------------*/
/* file: DrawableAxesJoGL.cpp                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw an       */
/*        Axes object with JoGL                                            */
/*------------------------------------------------------------------------*/

#include "DrawableAxesJoGL.hxx"

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DrawableAxesJoGL::DrawableAxesJoGL(DrawableAxes * Axes)
  : DrawableClippedObjectJoGL(Axes) 
{

}
/*---------------------------------------------------------------------------------*/
DrawableAxesJoGL::~DrawableAxesJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
DrawableAxes * DrawableAxesJoGL::getAxesDrawer( void )
{
  return dynamic_cast<DrawableAxes *>(getDrawer());
}
/*---------------------------------------------------------------------------------*/
}
