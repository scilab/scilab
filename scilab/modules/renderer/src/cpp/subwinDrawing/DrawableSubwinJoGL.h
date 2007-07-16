/*------------------------------------------------------------------------*/
/* file: DrawableSubwinJoGL.h                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        subwin object with JoGL                                         */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SUBWIN_JOGL_H_
#define _DRAWABLE_SUBWIN_JOGL_H_

#include "DrawableSubwinBridge.h"
#include "DrawableSubwin.h"
#include "../DrawableObjectJoGL.h"

namespace sciGraphics
{

  class DrawableSubwinJoGL : public virtual DrawableSubwinBridge, public DrawableObjectJoGL
  {

  public:

    DrawableSubwinJoGL( DrawableSubwin * drawer ) ;

    virtual ~DrawableSubwinJoGL( void ) ;

    /**
     * Return the drawn object
     */
    virtual DrawableSubwin * getSubwinDrawer( void ) ;

  protected:

    DrawableSubwin * m_pDrawed ;

  } ;

}

#endif /* _DRAWABLE_SUBWIN_JOGL_H_  */
