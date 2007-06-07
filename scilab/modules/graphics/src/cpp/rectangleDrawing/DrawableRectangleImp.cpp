/*------------------------------------------------------------------------*/
/* file: DrawableRectangleImp.cpp                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        rectangle object                                                */
/*------------------------------------------------------------------------*/


#include "DrawableRectangleImp.h"
#include "DrawableRectangle.h"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
DrawableRectangleImp::DrawableRectangleImp( DrawableRectangle * drawer ) : DrawableObjectImp( drawer ) {}
/*------------------------------------------------------------------------------------------*/
DrawableRectangle * DrawableRectangleImp::getDrawer( void )
{
  return dynamic_cast<DrawableRectangle *>(m_pDrawer) ;
}
/*------------------------------------------------------------------------------------------*/
}
