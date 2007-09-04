/*------------------------------------------------------------------------*/
/* file: PolylineLineDrawerJoGL.hxx                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the line of a polyline object                  */
/*------------------------------------------------------------------------*/

#ifndef _POLYLINE_LINE_DRAWER_JOGL_HXX_
#define _POLYLINE_LINE_DRAWER_JOGL_HXX_

#include "DrawablePolyline.h"
#include "DrawPolylineStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "PolylineLineDrawerJavaMapper.hxx"


namespace sciGraphics
{

class PolylineLineDrawerJoGL : public DrawPolylineStrategy, public DrawableObjectJoGL
{

public:

  PolylineLineDrawerJoGL( DrawablePolyline * polyline ) ;

  virtual ~PolylineLineDrawerJoGL(void);

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
  PolylineLineDrawerJavaMapper * getLineDrawerJavaMapper(void);


};

}

#endif /* _POLYLINE_LINE_DRAWER_JOGL_HXX_ */
