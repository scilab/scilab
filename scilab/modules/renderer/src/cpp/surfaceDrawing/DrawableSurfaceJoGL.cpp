/*------------------------------------------------------------------------*/
/* file: DrawableSurfaceJoGL.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw a        */
/*        surface object with JoGL                                        */
/*------------------------------------------------------------------------*/

#include "DrawableSurfaceJoGL.hxx"
#include "DrawableSurface.h"
#include "DrawableSurfaceJavaMapper.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableSurfaceJoGL::DrawableSurfaceJoGL( DrawableSurface * drawer )
  : DrawableClippedObjectJoGL(drawer)
{
  setJavaMapper(new DrawableSurfaceJavaMapper());
}
/*---------------------------------------------------------------------------------*/
DrawableSurfaceJoGL::~DrawableSurfaceJoGL( void )
{

}
/*---------------------------------------------------------------------------------*/
DrawableSurface * DrawableSurfaceJoGL::getSurfaceDrawer( void )
{
  return dynamic_cast<DrawableSurface *>(getDrawer()) ;
}
/*---------------------------------------------------------------------------------*/
DrawableSurfaceJavaMapper * DrawableSurfaceJoGL::getSurfaceJavaMapper(void)
{
  return dynamic_cast<DrawableSurfaceJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
