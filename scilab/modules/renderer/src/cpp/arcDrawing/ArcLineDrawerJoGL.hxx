/*------------------------------------------------------------------------*/
/* file: ArcLineDrawerJoGL.hxx                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the outline of an arc object                   */
/*------------------------------------------------------------------------*/

#ifndef _ARC_LINE_DRAWER_JOGL_HXX_
#define _ARC_LINE_DRAWER_JOGL_HXX_

#include "DrawableArc.h"
#include "DrawArcStrategy.h"
#include "DrawableObjectJoGL.h"
#include "ArcLineDrawerJavaMapper.hxx"


namespace sciGraphics
{

class ArcLineDrawerJoGL : public DrawArcStrategy, public DrawableObjectJoGL
{

public:

  ArcLineDrawerJoGL( DrawableArc * arc ) ;

  virtual ~ArcLineDrawerJoGL(void);

  /**
   * Main algorithm
   */
  virtual void drawArc( void );

  /**
   * Call the display list of an object
   */
  virtual void showArc( void );

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  ArcLineDrawerJavaMapper * getLineDrawerJavaMapper(void);


};

}

#endif /* _ARC_LINE_DRAWER_JOGL_HXX_ */
