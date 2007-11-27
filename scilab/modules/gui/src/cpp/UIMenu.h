/*------------------------------------------------------------------------*/
/* file: UIMenu.h                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Vincent COUVERT                                          */
/* desc : Class containing the drawing routine for a uimenu object     */
/*------------------------------------------------------------------------*/

#ifndef _UIMENU_H_
#define _UIMENU_H_


#include "../../../renderer/src/cpp/DrawableObject.h"
#include "UIMenuBridge.h"
#include "../jni/SwingScilabMenu.hxx"
extern "C" {
#include "getScilabJavaVM.h"
#include "../../../graphics/includes/GetProperty.h"
#include "../../../graphics/includes/CurrentObjectsManagement.h"
}

namespace sciGraphics
{

class UIMenu : public DrawableObject
{

public:

  UIMenu( sciPointObj * pObj ) : DrawableObject( pObj ) {}
//   {
//      std::cout << "UIMenu( sciPointObj * pObj ) : DrawableObject( pObj )" << std::endl;
//      org_scilab_modules_gui_bridge_menu::SwingScilabMenu * menu = new org_scilab_modules_gui_bridge_menu::SwingScilabMenu(getScilabJavaVM());
//      //menu->setText(pUIMENU_FEATURE (pObj)->label);
//      menu->setParent(sciGetNum(sciGetCurrentFigure()));
//      //menu->setCallback(pUIMENU_FEATURE (pObj)->callback);
//      //    pUIMENU_FEATURE (pObj)->hashMapIndex = menu->getHashMapIndex();
//    }

  virtual ~UIMenu( void ) ;

  /**
   * Return the real type of implementation object
   */
  UIMenuBridge * getUIMenuImp( void ) ;

protected:

  /*---------------------------------------------------------------------------------*/
  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  void draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  void show( void ) ;

  /**
   * Actually draw the uimenu on the screen
   */
  void drawUIMenu( void ) ;
  /*---------------------------------------------------------------------------------*/

} ;

}

#endif /* _DRAWABLE_UIMENU_H_  */
