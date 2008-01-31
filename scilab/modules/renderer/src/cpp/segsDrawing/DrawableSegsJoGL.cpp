/*------------------------------------------------------------------------*/
/* file: DrawableSegsJoGL.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        segs object with JoGL                                           */
/*------------------------------------------------------------------------*/

#include "DrawableSegsJoGL.hxx"
#include "DrawableSegs.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableSegsJoGL::DrawableSegsJoGL( DrawableSegs * drawer )
  : DrawableClippedObjectJoGL(drawer)
{
  
}
/*---------------------------------------------------------------------------------*/
DrawableSegsJoGL::~DrawableSegsJoGL( void )
{
  
}
/*---------------------------------------------------------------------------------*/
DrawableSegs * DrawableSegsJoGL::getSegsDrawer( void )
{
  return dynamic_cast<DrawableSegs *>(getDrawer()) ;
}
/*---------------------------------------------------------------------------------*/
DrawableSegsJavaMapper * DrawableSegsJoGL::getSegsJavaMapper(void)
{
  return dynamic_cast<DrawableSegsJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
