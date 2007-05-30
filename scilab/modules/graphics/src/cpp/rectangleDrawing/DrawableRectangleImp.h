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

namespace sciGraphics
{

  /**
  * To avoid cyclic dependances.
  */
  class DrawableRectangle ;

  class DrawableRectangleImp : virtual public DrawableObjectImp
  {

  public:

    DrawableRectangleImp( DrawableRectangle * drawer ) { m_pDrawer = drawer ; }

    virtual ~DrawableRectangleImp( void ) {}

    /**
     * Tell the canvas to draw itself.
     */
    virtual void drawRectangle( void ) = 0 ;

    /**
     * Return the drawed object
     */
    DrawableRectangle * getDrawer( void ) { return m_pDrawer ; }

  protected:

    DrawableRectangle * m_pDrawer ; /**< bridge for driver dependent algorithm */

  } ;

}

#endif /* _DRAWABLE_RECTANGLE_IMP_H_  */
