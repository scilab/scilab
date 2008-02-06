/*------------------------------------------------------------------------*/
/* file: DrawableFecJoGL.cpp                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw a        */
/*        fec object with JoGL                                            */
/*------------------------------------------------------------------------*/


#include "DrawableFecJoGL.hxx"
#include "DrawableFec.h"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
DrawableFecJoGL::DrawableFecJoGL( DrawableFec * drawer )
  : DrawableClippedObjectJoGL(drawer)
{

}
/*---------------------------------------------------------------------------------*/
DrawableFecJoGL::~DrawableFecJoGL( void )
{

}
/*---------------------------------------------------------------------------------*/
DrawableFec * DrawableFecJoGL::getFecDrawer( void )
{
  return dynamic_cast<DrawableFec *>(getDrawer()) ;
}
/*---------------------------------------------------------------------------------*/
DrawableFecJavaMapper * DrawableFecJoGL::getFecJavaMapper(void)
{
  return dynamic_cast<DrawableFecJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
