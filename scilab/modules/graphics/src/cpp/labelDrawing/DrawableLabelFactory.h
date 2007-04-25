/*------------------------------------------------------------------------*/
/* file: DrawableLabelFactory.h                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for label objects. Automaticaly create a label          */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_LABEL_FACTORY_H_
#define _DRAWABLE_LABEL_FACTORY_H_


#include "../DrawableObjectFactory.h"

class DrawableLabelFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

} ;

#endif /* _DRAWABLE_LABEL_FACTORY_H_  */
