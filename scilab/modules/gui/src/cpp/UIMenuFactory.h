/*------------------------------------------------------------------------*/
/* file: UIMenuFactory.h                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for rectangle objects. Automaticaly create a rectangle  */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _UIMENU_FACTORY_H_
#define _UIMENU_FACTORY_H_


#include "../../../renderer/src/cpp/DrawableObjectFactory.h"

namespace sciGraphics
{

class UIMenuFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

  /**
   * Update the drawer of the drawed object.
   * To be used when an object was deeply modified and to avoid to create it again completely.
   */
  virtual void update( void ) ;

} ;

}

#endif /* _UIMENU_FACTORY_H_  */

