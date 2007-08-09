/*------------------------------------------------------------------------*/
/* file: ArcFillDrawerJoGL.hxx                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the inside of an arc object                    */
/*------------------------------------------------------------------------*/

#ifndef _ARC_FILL_DRAWER_JOGL_HXX_
#define _ARC_FILL_DRAWER_JOGL_HXX_

#include "DrawableArc.h"
#include "DrawArcStrategy.h"
#include "DrawableObjectJoGL.h"
#include "ArcFillDrawerJavaMapper.hxx"


namespace sciGraphics
{

/**
 * Strategy drawing the inside of an arc object
 */
class ArcFillDrawerJoGL : public DrawArcStrategy, public DrawableObjectJoGL
{

public:

  ArcFillDrawerJoGL( DrawableArc * arc ) ;

  virtual ~ArcFillDrawerJoGL(void);

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
  ArcFillDrawerJavaMapper * getFillDrawerJavaMapper(void);


};

}

#endif /* _ARC_FILL_DRAWER_JOGL_HXX_ */
