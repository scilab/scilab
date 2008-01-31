/*------------------------------------------------------------------------*/
/* file: SegsMarkDrawerJoGL.hxx                                           */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Startegy drawing the marks at the end of segment                */
/*------------------------------------------------------------------------*/

#ifndef _SEGS_MARK_DRAWER_JOGL_HXX_
#define _SEGS_MARK_DRAWER_JOGL_HXX_

#include "DrawSegsStrategy.hxx"
#include "SegsMarkDrawerJavaMapper.hxx"
#include "../DrawableObjectJoGL.h"

namespace sciGraphics
{

  class SegsMarkDrawerJoGL : public DrawSegsStrategy, public DrawableObjectJoGL
  {
  public:

    SegsMarkDrawerJoGL( DrawableSegs * segs ) ;

    virtual ~SegsMarkDrawerJoGL(void);

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
    SegsMarkDrawerJavaMapper * getMarkDrawerJavaMapper(void);

  };

}

#endif /* _SEGS_MARK_DRAWER_JOGL_HXX_ */
