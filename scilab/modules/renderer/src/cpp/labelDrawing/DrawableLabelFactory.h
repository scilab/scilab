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
#include "DrawableLabel.h"

namespace sciGraphics
{

class DrawableLabelFactory : public DrawableObjectFactory
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

protected:

  /**
   * Set the kind of label positionner.
   */
  void setPositionner(DrawableLabel * label);

} ;

}

#endif /* _DRAWABLE_LABEL_FACTORY_H_  */
