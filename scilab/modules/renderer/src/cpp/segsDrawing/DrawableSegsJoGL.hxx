/*------------------------------------------------------------------------*/
/* file: DrawableSegseJoGL.hxx                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependant routines to draw a        */
/*        segs object with JoGL                                           */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_SEGS_JOGL_HXX_
#define _DRAWABLE_SEGS_JOGL_HXX_


#include "DrawableSegsBridge.hxx"
#include "DrawableSegs.h"
#include "DrawableSegsJavaMapper.hxx"
#include "../DrawableClippedObjectJoGL.h"

namespace sciGraphics
{

class DrawableSegsJoGL : public DrawableSegsBridge, public DrawableClippedObjectJoGL
{

public:

  DrawableSegsJoGL( DrawableSegs * drawer ) ;

  virtual ~DrawableSegsJoGL( void ) ;

  /**
   * Return the drawn object
   */
  DrawableSegs * getSegsDrawer( void ) ;


protected:

  /**
   * Get the object performing mapping with Java class.
   */
  DrawableSegsJavaMapper * getSegsJavaMapper(void);

} ;

}

#endif /* _DRAWABLE_SEGS_JOGL_HXX_  */

