/*------------------------------------------------------------------------*/
/* file: DrawableFecJoGL.hxx                                              */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the driver dependent routines to draw a        */
/*        fec object with JoGL                                            */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_FEC_JOGL_HXX_
#define _DRAWABLE_FEC_JOGL_HXX_

#include "DrawableFecBridge.hxx"
#include "DrawableFec.h"
#include "DrawableFecJavaMapper.hxx"
#include "../DrawableClippedObjectJoGL.h"

namespace sciGraphics
{

class DrawableFecJoGL : public DrawableFecBridge, public DrawableClippedObjectJoGL
{

public:
  DrawableFecJoGL( DrawableFec * drawer ) ;

  virtual ~DrawableFecJoGL( void ) ;

  /**
  * Return the drawn object
  */
  DrawableFec * getFecDrawer( void ) ;

protected:

  /**
  * Get the object performing mapping with Java class.
  */
  DrawableFecJavaMapper * getFecJavaMapper(void);

};

}

#endif /* _DRAWABLE_FEC_JOGL_HXX_ */
