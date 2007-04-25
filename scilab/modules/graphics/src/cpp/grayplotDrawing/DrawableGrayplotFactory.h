/*------------------------------------------------------------------------*/
/* file: DrawableGrayplotFactory.h                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for grayplot objects. Automaticaly create a grayplot    */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_GRAYPLOT_FACTORY_H_
#define _DRAWABLE_GRAYPLOT_FACTORY_H_


#include "../DrawableObjectFactory.h"

namespace sciGraphics
{

class DrawableGrayplotFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

} ;

}

#endif /* _DRAWABLE_GRAYPLOT_FACTORY_H_  */
