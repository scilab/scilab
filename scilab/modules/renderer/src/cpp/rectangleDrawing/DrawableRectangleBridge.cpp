/*------------------------------------------------------------------------*/
/* file: DrawableRectangleBridge.cpp                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        rectangle object                                                */
/*------------------------------------------------------------------------*/


#include "DrawableRectangleBridge.h"
#include "DrawableRectangle.h"

namespace sciGraphics
{

using namespace std ;

/*---------------------------------------------------------------------------------*/
DrawableRectangleBridge::DrawableRectangleBridge( void )
{
  m_oDrawingStrategies.clear() ;
}
/*---------------------------------------------------------------------------------*/
DrawableRectangleBridge::~DrawableRectangleBridge( void )
{
  // delete the strategies
  removeDrawingStrategies() ;
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangleBridge::drawRectangle( void )
{
  list<DrawRectangleStrategy *>::iterator it = m_oDrawingStrategies.begin() ;
  for ( ; it != m_oDrawingStrategies.end() ; it++ )
  {
    (*it)->drawRectangle() ;
  }
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangleBridge::addDrawingStrategy( DrawRectangleStrategy * strategy )
{
  m_oDrawingStrategies.push_back( strategy ) ;
}
/*---------------------------------------------------------------------------------*/
void DrawableRectangleBridge::removeDrawingStrategies( void )
{
  list<DrawRectangleStrategy *>::iterator it = m_oDrawingStrategies.begin() ;
  for ( ; it != m_oDrawingStrategies.end() ; it++ )
  {
    delete *it ;
    *it = NULL ;
  }
  m_oDrawingStrategies.clear() ;
}
/*---------------------------------------------------------------------------------*/
}
