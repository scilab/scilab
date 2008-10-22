/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "DecomposeSegsStrategy.hxx"

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
DecomposeSegsStrategy::DecomposeSegsStrategy(DrawableSegs * segs)
{
  m_pDrawed = segs;
}
/*---------------------------------------------------------------------------------*/
DecomposeSegsStrategy::~DecomposeSegsStrategy(void)
{
  m_pDrawed = NULL;
}
/*---------------------------------------------------------------------------------*/
void DecomposeSegsStrategy::getBoundingBox(double bounds[6])
{
  // TODO optimized the algorithm which is a bit rough
  int nbSegs = getNbSegment();
  double * xStarts = new double[nbSegs];
  double * xEnds = new double[nbSegs];
  double * yStarts  = new double[nbSegs];
  double * yEnds = new double[nbSegs];
  double * zStarts = new double[nbSegs];
  double * zEnds = new double[nbSegs];

  // find position of segs
  getSegsPos(xStarts, xEnds, yStarts, yEnds, zStarts, zEnds);


  // now get bounding box
  bounds[0] = xStarts[0]; // init xMin
  bounds[1] = xStarts[0]; // init xMax
  bounds[2] = yStarts[0]; // init yMin
  bounds[3] = yStarts[0]; // init yMax
  bounds[4] = zStarts[0]; // init zMin
  bounds[5] = zStarts[0]; // init zMax

  // find xMin and xMax within xStarts and xEnds
  // same for yMin and yMax
  for (int i = 0; i < nbSegs; i++)
  {
    updateMinMax(bounds[0], bounds[1], xStarts[i]);
    updateMinMax(bounds[0], bounds[1], xEnds[i]);

    updateMinMax(bounds[2], bounds[3], yStarts[i]);
    updateMinMax(bounds[2], bounds[3], yEnds[i]);

    updateMinMax(bounds[4], bounds[5], zStarts[i]);
    updateMinMax(bounds[4], bounds[5], zEnds[i]);

  }


  delete[] xStarts;
  delete[] xEnds;
  delete[] yStarts;
  delete[] yEnds;
  delete[] zStarts;
  delete[] zEnds;
}
/*---------------------------------------------------------------------------------*/
void DecomposeSegsStrategy::updateMinMax(double & curMin, double & curMax, double newVal)
{
  if (newVal < curMin)
  {
    curMin = newVal;
  }
  else if (newVal > curMax)
  {
    curMax = newVal;
  }
}
/*---------------------------------------------------------------------------------*/
}
