/*------------------------------------------------------------------------*/
/* file: DrawableAxesJoGL.hxx                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw an       */
/*        Axes object with JoGL                                            */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_AXES_JOGL_HXX_
#define _DRAWABLE_AXES_JOGL_HXX_

#include "DrawableAxes.h"
#include "DrawableAxesBridge.hxx"
#include "DrawableClippedObjectJoGL.h"

namespace sciGraphics
{

class DrawableAxesJoGL : public virtual DrawableAxesBridge, public DrawableClippedObjectJoGL
{
public:

  DrawableAxesJoGL(DrawableAxes * Axes) ;

  virtual ~DrawableAxesJoGL(void);

  /**
   * Return the drawn object
   */
  virtual DrawableAxes * getAxesDrawer( void );

protected:


};

}

#endif /* _DRAWABLE_AXES_JOGL_HXX_ */
