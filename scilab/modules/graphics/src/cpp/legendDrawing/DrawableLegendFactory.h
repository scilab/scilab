/*------------------------------------------------------------------------*/
/* file: DrawableLegendFactory.h                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for legend objects. Automaticaly create a legend        */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_LEGEND_FACTORY_H_
#define _DRAWABLE_LEGEND_FACTORY_H_


#include "../DrawableObjectFactory.h"

class DrawableLegendFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

} ;

#endif /* _DRAWABLE_LEGEND_FACTORY_H_  */
