/*------------------------------------------------------------------------*/
/* file: DrawablePolylineJoGL.h                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        polyline object with JoGL                                       */
/*------------------------------------------------------------------------*/

#include "DrawablePolylineJoGL.h"
#include "DrawablePolyline.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawablePolylineJoGL::DrawablePolylineJoGL( DrawablePolyline * drawer )
  : DrawableClippedObjectJoGL(drawer)
{
  
}
/*---------------------------------------------------------------------------------*/
DrawablePolylineJoGL::~DrawablePolylineJoGL( void )
{
  
}
/*---------------------------------------------------------------------------------*/
DrawablePolyline * DrawablePolylineJoGL::getPolylineDrawer( void )
{
  return dynamic_cast<DrawablePolyline *>(getDrawer()) ;
}
/*---------------------------------------------------------------------------------*/
DrawablePolylineJavaMapper * DrawablePolylineJoGL::getPolylineJavaMapper(void)
{
  return dynamic_cast<DrawablePolylineJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
