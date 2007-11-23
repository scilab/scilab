/*------------------------------------------------------------------------*/
/* file: DrawableSubwinJoGL.cpp                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        subwin object with JoGL                                         */
/*------------------------------------------------------------------------*/


#include "DrawableSubwinJoGL.h"
#include "DrawableSubwin.h"
#include "../DrawableObjectJoGL.h"
#include "../DrawableObjectBridge.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableSubwinJoGL::DrawableSubwinJoGL( DrawableSubwin * drawer )
  : DrawableObjectJoGL(drawer)
{

}
/*---------------------------------------------------------------------------------*/
DrawableSubwinJoGL::~DrawableSubwinJoGL( void )
{

}
/*---------------------------------------------------------------------------------*/
void DrawableSubwinJoGL::drawBox(const double bounds[6])
{
  getSubwinJavaMapper()->drawBox(bounds[0], bounds[1], bounds[2], bounds[3], bounds[4], bounds[5]);
}
/*---------------------------------------------------------------------------------*/
DrawableSubwin * DrawableSubwinJoGL::getSubwinDrawer( void )
{
  return dynamic_cast<DrawableSubwin *>(getDrawer()) ;
}
/*---------------------------------------------------------------------------------*/
DrawableSubwinJavaMapper * DrawableSubwinJoGL::getSubwinJavaMapper(void)
{
  return dynamic_cast<DrawableSubwinJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
