/*------------------------------------------------------------------------*/
/* file: DrawableRectangleImp.h                                           */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        rectangle object                                                */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_RECTANGLE_IMP_H_
#define _DRAWABLE_RECTANGLE_IMP_H_

#include "../DrawableObject.h"
#include "../DrawableObjectImp.h"
#include "DrawRectangleStrategy.h"
#include <list>

namespace sciGraphics
{

  /**
  * To avoid cyclic dependances.
  */
  class DrawableRectangle ;

  class DrawableRectangleImp : public virtual DrawableObjectImp
  {

  public:

    DrawableRectangleImp( DrawableRectangle * drawer ) ;

    virtual ~DrawableRectangleImp( void ) ;

    /**
     * Tell the canvas to draw itself.
     */
    virtual void drawRectangle( void ) ;

    /**
     * Return the drawed object
     */
    DrawableRectangle * getDrawer( void ) ;

    /**
    * Add a rendering algorithm
    */
    void addDrawingStrategy( DrawRectangleStrategy * strategy ) ;

    /**
    * Remove all drawing strategies.
    */
    void removeDrawingStrategies( void ) ;

  protected:

    /**
    * List of different rendering algorithms to perform
    */
    std::list<DrawRectangleStrategy *> m_oDrawingStrategies ;

  } ;

}

#endif /* _DRAWABLE_RECTANGLE_IMP_H_  */
