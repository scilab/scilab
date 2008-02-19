/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Startegy drawing the head of segment arrows
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SegsLengthDependentArrowDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
#include "math_graphics.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
SegsLengthDependentArrowDrawerJoGL::SegsLengthDependentArrowDrawerJoGL( DrawableSegs * polyline )
  : SegsArrowDrawerJoGL(polyline)
{
  m_aXStarts = NULL;
  m_aXEnds = NULL;
  m_aYStarts = NULL;
  m_aYEnds = NULL;
  m_aZStarts = NULL;
  m_aZEnds = NULL;
  m_iNbSegs = 0;
}
/*---------------------------------------------------------------------------------*/
SegsLengthDependentArrowDrawerJoGL::~SegsLengthDependentArrowDrawerJoGL(void)
{
  m_aXStarts = NULL;
  m_aXEnds = NULL;
  m_aYStarts = NULL;
  m_aYEnds = NULL;
  m_aZStarts = NULL;
  m_aZEnds = NULL;
  m_iNbSegs = 0;
}
/*---------------------------------------------------------------------------------*/
void SegsLengthDependentArrowDrawerJoGL::drawSegs(const double xStarts[], const double xEnds[],
                                                  const double yStarts[], const double yEnds[],
                                                  const double zStarts[], const double zEnds[],
                                                  const int colors[], int nbSegment)
{

  // save positions data
  m_aXStarts = xStarts;
  m_aXEnds = xEnds;
  m_aYStarts = yStarts;
  m_aYEnds = yEnds;
  m_aZStarts = zStarts;
  m_aZEnds = zEnds;
  m_iNbSegs = nbSegment;

  sciPointObj * pSegs = m_pDrawed->getDrawedObject();

  // find max length
  double maxLength = getMaxLength();
  // create a vector of arrow sizes
  double * arrowSizes = new double[nbSegment];

  // arrow size is negative there
  double maxArrowSize = - sciGetArrowSize(pSegs);
  double lineWidth = sciGetLineWidth(pSegs);

  // fill it
  // the longest vector has the specified length
  // for the others its proportional to their length
  for (int i = 0; i < nbSegment; i++)
  {
    arrowSizes[i] = lineWidth * maxArrowSize * getVectorLength(i) / maxLength;
  }

  initializeDrawing();

  getArrowDrawerJavaMapper()->setArrowSizes(arrowSizes, nbSegment);
  delete[] arrowSizes;

  double bounds[6];
  sciGetRealDataBounds(sciGetParentSubwin(pSegs), bounds);
  getArrowDrawerJavaMapper()->setAxesBounds(bounds[0], bounds[1],
                                            bounds[2], bounds[3],
                                            bounds[4], bounds[5]);

  getArrowDrawerJavaMapper()->drawSegs(xStarts, xEnds,
                                       yStarts, yEnds,
                                       zStarts, zEnds,
                                       colors, nbSegment);
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
double SegsLengthDependentArrowDrawerJoGL::getVectorLength(int index)
{
  double vect[3] = {m_aXEnds[index] - m_aXStarts[index],
                    m_aYEnds[index] - m_aYStarts[index],
                    m_aZEnds[index] - m_aZStarts[index]};
  return NORM_3D(vect);
}
/*---------------------------------------------------------------------------------*/
double SegsLengthDependentArrowDrawerJoGL::getMaxLength(void)
{
  double maxLength = 0;
  for(int i = 0; i < m_iNbSegs; i++)
  {
    double curLength = getVectorLength(i);
    if (curLength > maxLength)
    {
      maxLength = curLength;
    }
  }
  return maxLength;
}
/*---------------------------------------------------------------------------------*/
}
