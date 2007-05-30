/*------------------------------------------------------------------------*/
/* file: DrawableRectangleImpFactory.h                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for DrawableRectangleImp                                */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_RECTANGLE_IMP_FACTORY_H_
#define _DRAWABLE_RECTANGLE_IMP_FACTORY_H_


#include "DrawableRectangle.h"

namespace sciGraphics
{

  class DrawableRectangleImpFactory
  {

  public:

    DrawableRectangleImpFactory( void ) { m_pDrawable = NULL ; }

    virtual ~DrawableRectangleImpFactory( void ) {}

    /**
    * Create a new drawer from the drawed object.
    */
    virtual DrawableRectangleImp * create( void ) ;

    /**
    * Set the DrawableRectangle wich needs algorithms
    */
    void setDrawedRectangle( DrawableRectangle * rect ) ;

  protected:

    DrawableRectangle * m_pDrawable ;

  } ;

}

#endif /* _DRAWABLE_RECTANGLE_IMP_FACTORY_H_  */
