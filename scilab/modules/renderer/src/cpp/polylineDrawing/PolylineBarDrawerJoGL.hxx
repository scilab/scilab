/*------------------------------------------------------------------------*/
/* file: PolylineBarDrawerJoGL.hxx                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing bar plots                                      */
/*------------------------------------------------------------------------*/

#ifndef _POLYLINE_BAR_DRAWER_JOGL_HXX_
#define _POLYLINE_BAR_DRAWER_JOGL_HXX_

#include "DrawablePolyline.h"
#include "DrawPolylineStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "PolylineBarDrawerJavaMapper.hxx"


namespace sciGraphics
{

class PolylineBarDrawerJoGL : public DrawPolylineStrategy, public DrawableObjectJoGL
{

public:

  PolylineBarDrawerJoGL( DrawablePolyline * polyline ) ;

  virtual ~PolylineBarDrawerJoGL(void);

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
  PolylineBarDrawerJavaMapper * getBarDrawerJavaMapper(void);


};

}

#endif /* _POLYLINE_BAR_DRAWER_JOGL_HXX_ */
