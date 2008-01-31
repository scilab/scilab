/*------------------------------------------------------------------------*/
/* file: SegsLengthDependentArrowDrawerJoGL.hxx                           */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Startegy drawing the head of segment arrows. Head size depend   */
/*        one arrow length                                                */
/*------------------------------------------------------------------------*/

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
