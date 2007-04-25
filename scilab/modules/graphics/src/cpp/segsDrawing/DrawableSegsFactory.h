/*------------------------------------------------------------------------*/
/* file: DrawableSegsFactory.h                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for segs objects. Automaticaly create a segse           */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SEGS_FACTORY_H_
#define _DRAWABLE_SEGS_FACTORY_H_


#include "../DrawableObjectFactory.h"

class DrawableSegsFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

} ;

#endif /* _DRAWABLE_SEGS_FACTORY_H_  */
