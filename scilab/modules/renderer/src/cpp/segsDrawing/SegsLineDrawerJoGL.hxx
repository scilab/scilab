/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Startegy drawing the line part of segment arrows
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SEGS_LINE_DRAWER_JOGL_HXX_
#define _SEGS_LINE_DRAWER_JOGL_HXX_

#include "DrawSegsStrategy.hxx"
#include "SegsLineDrawerJavaMapper.hxx"
#include "../DrawableObjectJoGL.h"

namespace sciGraphics
{

class SegsLineDrawerJoGL : public DrawSegsStrategy, public DrawableObjectJoGL
{
public:

  SegsLineDrawerJoGL( DrawableSegs * segs ) ;

  virtual ~SegsLineDrawerJoGL(void);

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
  SegsLineDrawerJavaMapper * getLineDrawerJavaMapper(void);

};

}

#endif /* _SEGS_LINE_DRAWER_JOGL_HXX_ */

