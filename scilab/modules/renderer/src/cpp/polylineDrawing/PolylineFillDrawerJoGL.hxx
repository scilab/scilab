/*------------------------------------------------------------------------*/
/* file: PolylineFillDrawerJoGL.hxx                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy filling the inside of a polygon curve                  */
/*------------------------------------------------------------------------*/

#ifndef _POLYLINE_FILL_DRAWER_JOGL_HXX_
#define _POLYLINE_FILL_DRAWER_JOGL_HXX_

#include "DrawablePolyline.h"
#include "DrawPolylineStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "PolylineFillDrawerJavaMapper.hxx"


namespace sciGraphics
{

class PolylineFillDrawerJoGL : public DrawPolylineStrategy, public DrawableObjectJoGL
{

public:

  PolylineFillDrawerJoGL( DrawablePolyline * polyline ) ;

  virtual ~PolylineFillDrawerJoGL(void);

  /**
   * Main algorithm
   */
  virtual void drawPolyline( void );

  /**
   * Call the display list of an object
   */
  virtual void showPolyline( void );

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  PolylineFillDrawerJavaMapper * getFillDrawerJavaMapper(void);


};

}

#endif /* _POLYLINE_FILL_DRAWER_JOGL_HXX_ */
