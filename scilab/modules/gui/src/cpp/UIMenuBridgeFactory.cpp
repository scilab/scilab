/*------------------------------------------------------------------------*/
/* file: UIMenuBridgeFactory.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                              */
/* desc : Factory for UIMenuImp                                           */
/*------------------------------------------------------------------------*/

#include "UIMenuBridgeFactory.h"
#include "UIMenuJoGL.h"
//#include "UIMenuFillDrawerJoGL.h"
//#include "UIMenuLineDrawerJoGL.h"
//#include "UIMenuMarkDrawerJoGL.h"
#include "UIMenuJavaMapper.hxx"
//#include "UIMenuFillDrawerJavaMapper.hxx"
//#include "UIMenuLineDrawerJavaMapper.hxx"
//#include "UIMenuMarkDrawerJavaMapper.hxx"

extern "C"
{
#include "../../../graphics/includes/GetProperty.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
UIMenuBridge * UIMenuBridgeFactory::create( void )
{
  UIMenuJoGL * imp = new UIMenuJoGL( m_pDrawable ) ;
  
  imp->setJavaMapper(new UIMenuJavaMapper());

  setStrategies(imp) ;

  return imp ;
}
/*------------------------------------------------------------------------------------------*/
void UIMenuBridgeFactory::update( void )
{
  setStrategies( m_pDrawable->getUIMenuImp() ) ;
}
/*------------------------------------------------------------------------------------------*/
void UIMenuBridgeFactory::setDrawedUIMenu( UIMenu * uimenu )
{
  m_pDrawable = uimenu ;
}
/*------------------------------------------------------------------------------------------*/
void UIMenuBridgeFactory::setStrategies( UIMenuBridge * imp )
{
  imp->removeDrawingStrategies() ;

  // Create rendering algorithms
  // TODO add statements...
}
/*------------------------------------------------------------------------------------------*/

}
