/*------------------------------------------------------------------------*/
/* file: UIMenuJoGL.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        rectangle object with JoGL                                      */
/*------------------------------------------------------------------------*/

#include <list>
#include "../../../renderer/src/cpp/DrawableObjectJoGL.h"
#include "UIMenu.h"
#include "UIMenuJoGL.h"


namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
UIMenuJoGL::UIMenuJoGL( UIMenu * drawer )
  : DrawableObjectJoGL(drawer)
{

}
/*------------------------------------------------------------------------------------------*/
UIMenuJoGL::~UIMenuJoGL( void )
{

}
/*------------------------------------------------------------------------------------------*/
void UIMenuJoGL::show( void )
{
  std::list<DrawUIMenuStrategy *>::iterator it = m_oDrawingStrategies.begin() ;
  for ( ; it != m_oDrawingStrategies.end() ; it++ )
  {
    (*it)->show() ;
  }
}
/*------------------------------------------------------------------------------------------*/
UIMenu * UIMenuJoGL::getUIMenuDrawer( void )
{
  return dynamic_cast<UIMenu *>(getDrawer()) ;
}
/*------------------------------------------------------------------------------------------*/
UIMenuJavaMapper * UIMenuJoGL::getUIMenuJavaMapper(void)
{
  return dynamic_cast<UIMenuJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
void UIMenuJoGL::destroyUIMenu( void )
{
  DrawableObjectJoGL::destroy() ;
}
/*------------------------------------------------------------------------------------------*/
void UIMenuJoGL::setJavaMapper( UIMenuJavaMapper * javaMapper )
{
  m_pUIJavaMapper = javaMapper ;
}
/*------------------------------------------------------------------------------------------*/
}
