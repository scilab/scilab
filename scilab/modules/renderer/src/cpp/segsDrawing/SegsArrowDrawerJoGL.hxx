/*------------------------------------------------------------------------*/
/* file: SegsArrowDrawerJoGL.hxx                                          */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Startegy drawing the head of segment arrows                     */
/*------------------------------------------------------------------------*/

#ifndef _SEGS_ARROW_DRAWER_JOGL_HXX_
#define _SEGS_ARROW_DRAWER_JOGL_HXX_

#include "DrawSegsStrategy.hxx"
#include "SegsArrowDrawerJavaMapper.hxx"
#include "../DrawableObjectJoGL.h"

namespace sciGraphics
{

class SegsArrowDrawerJoGL : public DrawSegsStrategy, public DrawableObjectJoGL
{
public:

  SegsArrowDrawerJoGL( DrawableSegs * segs ) ;

  virtual ~SegsArrowDrawerJoGL(void);

  /**
   * Main algorithm, draw the segs from their positions
   */
  virtual void drawSegs(const double xStarts[], const double xEnds[],
                        const double yStarts[], const double yEnds[],
                        const double zStarts[], const double zEnds[],
                        const int colors[], int nbSegment);

  /**
   * Call the display list of an object
  */
  virtual void showSegs( void );

protected:

  /**
   * Get the object performing mapping with Java class.
   */
  SegsArrowDrawerJavaMapper * getArrowDrawerJavaMapper(void);

};

}

#endif /* _SEGS_ARROW_DRAWER_JOGL_HXX_ */
