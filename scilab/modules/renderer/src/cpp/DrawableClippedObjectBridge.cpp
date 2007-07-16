/*------------------------------------------------------------------------*/
/* file: DrawableClippedObjectBridge.cpp                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class with driver dependent algorithms of DrawableClippedObject */
/*        class                                                           */
/*------------------------------------------------------------------------*/


#include "DrawableClippedObjectBridge.h"
#include "DrawableClippedObject.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableClippedObject * DrawableClippedObjectBridge::getClippedObjectDrawer( void )
{
  return dynamic_cast<DrawableClippedObject *>(m_pDrawer);
}
/*------------------------------------------------------------------------------------------*/

}

