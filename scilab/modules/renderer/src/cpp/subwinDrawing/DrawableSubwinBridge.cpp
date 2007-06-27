/*------------------------------------------------------------------------*/
/* file: DrawableSubwinBridge.cpp                                         */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        subwin object                                                   */
/*------------------------------------------------------------------------*/

#include "DrawableSubwinBridge.h"
#include "DrawableSubwin.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableSubwinBridge::DrawableSubwinBridge( DrawableSubwin * drawer )
  : DrawableObjectImp(drawer)
{

}
/*------------------------------------------------------------------------------------------*/
DrawableSubwin * DrawableSubwinBridge::getDrawer( void )
{
  return dynamic_cast<DrawableSubwin *>(m_pDrawer) ;
}
/*------------------------------------------------------------------------------------------*/

}
