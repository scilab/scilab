/*------------------------------------------------------------------------*/
/* file: UIMenu.cpp                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                          */
/* desc : Class containing the drawing routine for a uimenu object     */
/*------------------------------------------------------------------------*/

#include "UIMenu.h"


namespace sciGraphics
{

using namespace std;

/*------------------------------------------------------------------------------------------*/
UIMenu::~UIMenu( void )
{

}
/*------------------------------------------------------------------------------------------*/
void UIMenu::draw( void )
{
//   initializeDrawing() ;
//   if ( !checkVisibility() )
//   {

//     endDrawing();
//     return ;
//   }
//   clip();
  drawUIMenu() ;
  std::cout << "void UIMenu::draw( void )" << std::endl;
//   unClip();
//   endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void UIMenu::show( void )
{
  //  clip();
  getUIMenuImp()->show() ;
  std::cout << "void UIMenu::show( void )" << std::endl;
  //unClip();
}
/*------------------------------------------------------------------------------------------*/
UIMenuBridge * UIMenu::getUIMenuImp( void )
{
  std::cout << "UIMenuBridge * UIMenu::getUIMenuImp( void )" << std::endl;
  return dynamic_cast<UIMenuBridge *>(m_pImp) ;
}
/*------------------------------------------------------------------------------------------*/
void UIMenu::drawUIMenu( void )
{
  std::cout << "void UIMenu::drawUIMenu( void )" << std::endl;
  getUIMenuImp()->drawUIMenu() ;
}
/*------------------------------------------------------------------------------------------*/

}
