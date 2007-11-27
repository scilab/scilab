/*------------------------------------------------------------------------*/
/* file: UIMenuFactory.cpp                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                              */
/* desc : Factory for uimenus objects. Automaticaly create a uimenu       */
/*        drawer with the right algorithms from the graphic handle        */
/*------------------------------------------------------------------------*/

#include "UIMenuFactory.h"
#include "UIMenuBridgeFactory.h"
#include "ConcreteUIMenu.h"
#include "getHandleDrawer.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
DrawableObject * UIMenuFactory::create( void )
{
  std::cout << "DrawableObject * UIMenuFactory::create( void )" << std::endl;
  ConcreteUIMenu * newUIMenu = new ConcreteUIMenu( m_pDrawed ) ;
  UIMenuBridgeFactory imp ;
  imp.setDrawedUIMenu( newUIMenu ) ;
  newUIMenu->setDrawableImp( imp.create() ) ;

  return newUIMenu ;
}
/*------------------------------------------------------------------------------------------*/
void UIMenuFactory::update( void )
{
  UIMenuBridgeFactory imp ;
  imp.setDrawedUIMenu( getUIMenuDrawer(m_pDrawed) ) ;
  imp.update();
}
/*------------------------------------------------------------------------------------------*/

}
