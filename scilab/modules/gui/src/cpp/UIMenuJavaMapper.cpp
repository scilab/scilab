/*------------------------------------------------------------------------*/
/* file: UIMenuJavaMapper.cpp                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by UIMenu       */
/*------------------------------------------------------------------------*/

#include "UIMenuJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
UIMenuJavaMapper::UIMenuJavaMapper(void)
{
  //m_pJavaObject = NULL;//new org_scilab_modules_gui_bridge_menu::SwingScilabMenu(getScilabJavaVM());
  //std::cout << "UIMenuJavaMapper::UIMenuJavaMapper(void)" << std::endl;
}
/*------------------------------------------------------------------------------------------*/
UIMenuJavaMapper::~UIMenuJavaMapper(void)
{
  //delete m_pJavaObject;
  //m_pJavaObject = NULL;
}
/*------------------------------------------------------------------------------------------*/
void UIMenuJavaMapper::display(void)
{
  //m_pJavaObject->display();
}
/*------------------------------------------------------------------------------------------*/
void UIMenuJavaMapper::show()
{
  //m_pJavaObject->show();
}
/*------------------------------------------------------------------------------------------*/
void UIMenuJavaMapper::destroy()
{
  //m_pJavaObject->destroy();
}
/*------------------------------------------------------------------------------------------*/
}
