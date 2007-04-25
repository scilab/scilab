/*------------------------------------------------------------------------*/
/* file: DrawableFigureFactory.h                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for figure objects. Automaticaly create a figure        */
/*        drawer with the right algorithms from the garphic handle        */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FIGURE_FACTORY_H_
#define _DRAWABLE_FIGURE_FACTORY_H_


#include "../DrawableObjectFactory.h"

class DrawableFigureFactory : public DrawableObjectFactory
{

public:

  /**
   * Create a new drawer from the drawed object.
   */
  DrawableObject * create( void ) ;

} ;

#endif /* _DRAWABLE_FIGURE_FACTORY_H_  */
