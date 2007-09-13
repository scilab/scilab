/*------------------------------------------------------------------------*/
/* file: DrawableArcJoGL.hxx                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw an       */
/*        arc object with JoGL                                            */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_ARC_JOGL_HXX_
#define _DRAWABLE_ARC_JOGL_HXX_

#include "DrawableArc.h"
#include "DrawableArcBridge.h"
#include "DrawableClippedObjectJoGL.h"

namespace sciGraphics
{
  
class DrawableArcJoGL : public virtual DrawableArcBridge, public DrawableClippedObjectJoGL
{
public:

  DrawableArcJoGL(DrawableArc * arc) ;

  virtual ~DrawableArcJoGL(void);

  /**
   * Return the drawn object
   */
  virtual DrawableArc * getArcDrawer( void );

protected:


};

}

#endif /* _DRAWABLE_ARC_JOGL_HXX_ */
