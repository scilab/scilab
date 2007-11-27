/*------------------------------------------------------------------------*/
/* file: UIMenuBridge.cpp                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        uimenu object                                                */
/*------------------------------------------------------------------------*/


#include "UIMenuBridge.h"
#include "UIMenu.h"

namespace sciGraphics
{

using namespace std ;

/*------------------------------------------------------------------------------------------*/
UIMenuBridge::UIMenuBridge( void )
{
  m_oDrawingStrategies.clear() ;
}
/*------------------------------------------------------------------------------------------*/
UIMenuBridge::~UIMenuBridge( void )
{
  // delete the strategies
  removeDrawingStrategies() ;
}
/*------------------------------------------------------------------------------------------*/
void UIMenuBridge::drawUIMenu( void )
{
  list<DrawUIMenuStrategy *>::iterator it = m_oDrawingStrategies.begin() ;
  for ( ; it != m_oDrawingStrategies.end() ; it++ )
  {
    (*it)->drawUIMenu() ;
  }
}
/*------------------------------------------------------------------------------------------*/
void UIMenuBridge::addDrawingStrategy( DrawUIMenuStrategy * strategy )
{
  m_oDrawingStrategies.push_back( strategy ) ;
}
/*------------------------------------------------------------------------------------------*/
void UIMenuBridge::removeDrawingStrategies( void )
{
  list<DrawUIMenuStrategy *>::iterator it = m_oDrawingStrategies.begin() ;
  for ( ; it != m_oDrawingStrategies.end() ; it++ )
  {
    delete *it ;
    *it = NULL ;
  }
  m_oDrawingStrategies.clear() ;
}
/*------------------------------------------------------------------------------------------*/
}
