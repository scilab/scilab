/*------------------------------------------------------------------------*/
/* file: PolylineMarkDrawerJoGL.hxx                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the marks on a polyline object                 */
/*------------------------------------------------------------------------*/


#ifndef _POLYLINE_MARK_DRAWER_JOGL_HXX_
#define _POLYLINE_MARK_DRAWER_JOGL_HXX_

#include "DrawablePolyline.h"
#include "DrawPolylineStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "PolylineMarkDrawerJavaMapper.hxx"


namespace sciGraphics
{

class PolylineMarkDrawerJoGL : public DrawPolylineStrategy, public DrawableObjectJoGL
{

public:

  PolylineMarkDrawerJoGL( DrawablePolyline * polyline ) ;

  virtual ~PolylineMarkDrawerJoGL(void);

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
  PolylineMarkDrawerJavaMapper * getMarkDrawerJavaMapper(void);


};

}

#endif /* _POLYLINE_LINE_DRAWER_JOGL_HXX_ */