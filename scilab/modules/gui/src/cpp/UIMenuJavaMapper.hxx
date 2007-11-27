/*------------------------------------------------------------------------*/
/* file: UIMenuJavaMapper.hxx                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                          */
/* desc : Contains mapping of java method used by UIMenu       */
/*------------------------------------------------------------------------*/

#ifndef _UIMENU_JAVA_MAPPER_HXX_
#define _UIMENU_JAVA_MAPPER_HXX_

//#include "../jni/SwingScilabMenu.hxx"

namespace sciGraphics
{

  class UIMenuJavaMapper
{
public:

  UIMenuJavaMapper(void) ;

  virtual ~UIMenuJavaMapper(void);

  /*----------------------------------------------------------------------*/
  virtual void display(void);

  virtual void show(void);

  virtual void destroy(void);
  /*----------------------------------------------------------------------*/

private:

  /**
   * Giws generated wrapper.
   */
  //org_scilab_modules_gui_bridge_menu::SwingScilabMenu * m_pJavaObject;

};

}

#endif
