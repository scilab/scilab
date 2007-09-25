/*------------------------------------------------------------------------*/
/* file: DrawableTextJoGL.cpp                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw a        */
/*        text object with JoGL                                           */
/*------------------------------------------------------------------------*/


#include "DrawableTextJoGL.hxx"
#include "DrawableText.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableTextJoGL::DrawableTextJoGL( DrawableText * drawer )
  : DrawableClippedObjectJoGL(drawer)
{

}
/*------------------------------------------------------------------------------------------*/
DrawableTextJoGL::~DrawableTextJoGL( void )
{

}
/*------------------------------------------------------------------------------------------*/
DrawableText * DrawableTextJoGL::getTextDrawer( void )
{
  return dynamic_cast<DrawableText *>(getDrawer()) ;
}
/*------------------------------------------------------------------------------------------*/
DrawableTextJavaMapper * DrawableTextJoGL::getTextJavaMapper(void)
{
  return dynamic_cast<DrawableTextJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}
