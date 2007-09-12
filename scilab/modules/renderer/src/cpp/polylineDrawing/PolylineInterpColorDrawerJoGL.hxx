/*------------------------------------------------------------------------*/
/* file: PolylineInterpColorDrawerJoGL.hxx                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy filling a polygon with interpolated colors             */
/*------------------------------------------------------------------------*/

#ifndef _POLYLINE_INTERP_COLOR_DRAWER_JOGL_HXX_
#define _POLYLINE_INTERP_COLOR_DRAWER_JOGL_HXX_

#include "DrawablePolyline.h"
#include "DrawPolylineStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "PolylineInterpColorDrawerJavaMapper.hxx"


namespace sciGraphics
{

class PolylineInterpColorDrawerJoGL : public DrawPolylineStrategy, public DrawableObjectJoGL
{

public:

  PolylineInterpColorDrawerJoGL( DrawablePolyline * polyline ) ;

  virtual ~PolylineInterpColorDrawerJoGL(void);

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
  PolylineInterpColorDrawerJavaMapper * getInterpColorDrawerJavaMapper(void);


};

}

#endif /* _POLYLINE_INTERP_COLOR_DRAWER_JOGL_HXX_ */
