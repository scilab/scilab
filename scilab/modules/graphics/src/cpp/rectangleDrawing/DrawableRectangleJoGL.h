/*------------------------------------------------------------------------*/
/* file: DrawableRectangleJoGL.h                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        rectangle object with JoGL                                      */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_RECTANGLE_JOGL_H_
#define _DRAWABLE_RECTANGLE_JOGL_H_

#include <jni.h>
#include <list>

#include "DrawableRectangleImp.h"
#include "DrawRectangleStrategyJoGL.h"
#include "../DrawableObjectJoGL.h"

namespace sciGraphics
{

  class DrawableRectangleJoGL : public DrawableRectangleImp, public DrawableObjectJoGL
  {

  public:

    DrawableRectangleJoGL( DrawableRectangle * drawer ) ;

    virtual ~DrawableRectangleJoGL( void ) ;

    /**
    * Tell the canvas to draw itself.
    */
    virtual void drawRectangle( void ) ;

    /**
     * Add a rendering algorithm
     */
    void addDrawingStrategy( DrawRectangleStrategyJoGL * strategy ) ;

  protected:

    /*-------------------------------------------------------------*/
    /** destroy the rectangle entity */
    void destroyRectangle( void ) ;
    /*-------------------------------------------------------------*/

    /**
     * List of different rendering algorithms to perform
     */
    std::list<DrawRectangleStrategyJoGL *> m_oDrawingStrategies ;
    /*-------------------------------------------------------------*/

    

  } ;

}

#endif /* _DRAWABLE_RECTANGLE_JOGL_H_  */

