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

using namespace std ;

/*------------------------------------------------------------------------------------------*/
DrawableRectangleImp::DrawableRectangleImp( DrawableRectangle * drawer ) : DrawableObjectImp( drawer )
{
  m_oDrawingStrategies.clear() ;
}
/*------------------------------------------------------------------------------------------*/
DrawableRectangleImp::~DrawableRectangleImp( void )
{
  // delete the strategies
  removeDrawingStrategies() ;
}
/*------------------------------------------------------------------------------------------*/
DrawableRectangle * DrawableRectangleImp::getDrawer( void )
{
  return dynamic_cast<DrawableRectangle *>(m_pDrawer) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleImp::drawRectangle( void )
{
  list<DrawRectangleStrategy *>::iterator it = m_oDrawingStrategies.begin() ;
  for ( ; it != m_oDrawingStrategies.end() ; it++ )
  {
    (*it)->drawRectangle() ;
  }
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleImp::addDrawingStrategy( DrawRectangleStrategy * strategy )
{
  m_oDrawingStrategies.push_back( strategy ) ;
}
/*------------------------------------------------------------------------------------------*/
void DrawableRectangleImp::removeDrawingStrategies( void )
{
  list<DrawRectangleStrategy *>::iterator it = m_oDrawingStrategies.begin() ;
  for ( ; it != m_oDrawingStrategies.end() ; it++ )
  {
    delete *it ;
    *it = NULL ;
  }
  m_oDrawingStrategies.clear() ;
}
/*------------------------------------------------------------------------------------------*/
}
