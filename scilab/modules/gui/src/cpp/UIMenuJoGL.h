/*------------------------------------------------------------------------*/
/* file: UIMenuJoGL.h                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        uimenu object with JoGL                                      */
/*------------------------------------------------------------------------*/

#ifndef _UIMENU_JOGL_H_
#define _UIMENU_JOGL_H_


#include "UIMenuBridge.h"
#include "UIMenu.h"
#include "UIMenuJavaMapper.hxx"
#include "../../../renderer/src/cpp/DrawableObjectJoGL.h"

namespace sciGraphics
{

class UIMenuJoGL : public UIMenuBridge, public DrawableObjectJoGL
{

public:

  UIMenuJoGL( UIMenu * drawer ) ;

  virtual ~UIMenuJoGL( void ) ;

  /**
   * Call the display list of an object
   */
  virtual void show( void ) ;

  /**
   * Return the drawn object
   */
  UIMenu * getUIMenuDrawer( void ) ;

  void setJavaMapper( UIMenuJavaMapper * javaMapper );

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  UIMenuJavaMapper * getUIMenuJavaMapper(void);
  /*-------------------------------------------------------------*/
  /** destroy the uimenu entity */
  void destroyUIMenu( void ) ;
  /*-------------------------------------------------------------*/

  protected:
  UIMenuJavaMapper* m_pUIJavaMapper;

} ;

}

#endif /* _UIMENU_JOGL_H_  */

