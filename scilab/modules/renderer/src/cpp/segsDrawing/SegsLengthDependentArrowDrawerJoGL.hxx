/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Startegy drawing the head of segment arrows. Head size depend
 * one arrow length
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _SEGS_ARROW_LENGTH_DEPENDENT_DRAWER_JOGL_HXX_
#define _SEGS_ARROW_LENGTH_DEPENDENT_DRAWER_JOGL_HXX_

#include "SegsArrowDrawerJoGL.hxx"
#include "SegsArrowDrawerJavaMapper.hxx"
#include "../DrawableObjectJoGL.h"

namespace sciGraphics
{

class SegsLengthDependentArrowDrawerJoGL : public SegsArrowDrawerJoGL
{
public:

  SegsLengthDependentArrowDrawerJoGL( DrawableSegs * polyline ) ;

  virtual ~SegsLengthDependentArrowDrawerJoGL(void);

  /**
  * Main algorithm, draw the segs from their positions
  */
  virtual void drawSegs(const double xStarts[], const double xEnds[],
                        const double yStarts[], const double yEnds[],
                        const double zStarts[], const double zEnds[],
                        const int colors[], int nbSegment);

protected:
  /*-----------------------------------------------------------------*/
  /**
   * Get the length of vector index.
   */
  double getVectorLength(int index);

  /**
   * Fin the length of the longest vector
   */
  double getMaxLength(void);
  /*-----------------------------------------------------------------*/
  int m_iNbSegs;
  const double * m_aXStarts;
  const double * m_aXEnds;
  const double * m_aYStarts;
  const double * m_aYEnds;
  const double * m_aZStarts;
  const double * m_aZEnds;
  /*-----------------------------------------------------------------*/


};

}

#endif /* _SEGS_ARROW_LENGTH_DEPENDENT_DRAWER_JOGL_HXX_ */

