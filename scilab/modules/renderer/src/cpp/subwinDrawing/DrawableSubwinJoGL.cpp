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
#include "../DrawableObjectImp.h"

extern "C"
{
#include "JniUtils.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableSubwinJoGL::DrawableSubwinJoGL( DrawableSubwin * drawer )
  : DrawableObjectJoGL(drawer, "org/scilab/modules/renderer/subwinDrawing/DrawableSubwinJoGL")
{

}
/*------------------------------------------------------------------------------------------*/
DrawableSubwinJoGL::~DrawableSubwinJoGL( void )
{

}
/*------------------------------------------------------------------------------------------*/
DrawableSubwin * DrawableSubwinJoGL::getSubwinDrawer( void )
{
  return dynamic_cast<DrawableSubwin *>(getDrawer()) ;
}
/*------------------------------------------------------------------------------------------*/

}
