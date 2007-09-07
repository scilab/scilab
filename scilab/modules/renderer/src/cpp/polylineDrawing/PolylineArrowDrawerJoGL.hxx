/*------------------------------------------------------------------------*/
/* file: PolylineArrowDrawerJoGL.hxx                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing arrow heads on a polyline object               */
/*------------------------------------------------------------------------*/

#ifndef _POLYLINE_ARROW_DRAWER_JOGL_HXX_
#define _POLYLINE_ARROW_DRAWER_JOGL_HXX_

#include "DrawablePolyline.h"
#include "DrawPolylineStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "PolylineArrowDrawerJavaMapper.hxx"


namespace sciGraphics
{

class PolylineArrowDrawerJoGL : public DrawPolylineStrategy, public DrawableObjectJoGL
{

public:

  PolylineArrowDrawerJoGL( DrawablePolyline * polyline ) ;

  virtual ~PolylineArrowDrawerJoGL(void);

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
  PolylineArrowDrawerJavaMapper * getArrowDrawerJavaMapper(void);


};

}

#endif /* _POLYLINE_ARROW_DRAWER_JOGL_HXX_ */
