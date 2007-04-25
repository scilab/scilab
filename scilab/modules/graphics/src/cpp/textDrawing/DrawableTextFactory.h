/*------------------------------------------------------------------------*/
/* file: DrawableTextFactory.h                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for text objects. Automaticaly create a text            */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_TEXT_FACTORY_H_
#define _DRAWABLE_TEXT_FACTORY_H_


#include "../DrawableObjectFactory.h"

class DrawableTextFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

} ;

#endif /* _DRAWABLE_TEXT_FACTORY_H_  */
