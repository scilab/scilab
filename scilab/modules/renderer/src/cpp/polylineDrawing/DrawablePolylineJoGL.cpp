/*------------------------------------------------------------------------*/
/* file: DrawablePolylineJoGL.h                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        polyline object with JoGL                                       */
/*------------------------------------------------------------------------*/

#include "DrawablePolylineJoGL.h"
#include "DrawablePolyline.h"

extern "C"
{
#include "JniUtils.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawablePolylineJoGL::DrawablePolylineJoGL( DrawablePolyline * drawer )
: DrawableObjectImp(drawer), DrawablePolylineBridge(drawer), DrawableObjectJoGL(drawer, "org/scilab/modules/renderer/polylineDrawing/DrawablePolylineJoGL")
{

}
/*------------------------------------------------------------------------------------------*/
DrawablePolylineJoGL::~DrawablePolylineJoGL( void )
{

}
/*------------------------------------------------------------------------------------------*/

}
