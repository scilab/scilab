/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Decompose polyline to get data for horizontal bar drawing
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "HorizontalBarDecomposition.hxx"


extern "C"
{
#include "BasicAlgos.h"
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
HorizontalBarDecomposition::HorizontalBarDecomposition( BarDecomposition * decomposition )
  : LinearBarDecomposition(decomposition)
{

}
/*---------------------------------------------------------------------------------*/
HorizontalBarDecomposition::~HorizontalBarDecomposition( void )
{

}
/*---------------------------------------------------------------------------------*/
void HorizontalBarDecomposition::getBarOrdinates(double bottom[], double top[])
{
  sciPointObj * pPolyline = m_pDecomposition->getDrawedPolyline()->getDrawedObject();
  int nbVertices = m_pDecomposition->getDrawnVerticesLength();
  double * yPoints = pPOLYLINE_FEATURE(pPolyline)->pvy;
  double * yShift = pPOLYLINE_FEATURE(pPolyline)->y_shift;
  double barWidth = pPOLYLINE_FEATURE(pPolyline)->bar_width;


  for (int i = 0; i < nbVertices; i++)
  {
    bottom[i] = yPoints[i] - barWidth / 2.0;
    top[i] = yPoints[i] + barWidth / 2.0;
  }

  if (yShift != NULL)
  {
    for (int i = 0; i < nbVertices; i++)
    {
      bottom[i] += yShift[i];
      top[i] += yShift[i];
    }
  }

}
/*---------------------------------------------------------------------------------*/
void HorizontalBarDecomposition::getBarAbscissas(double left[], double right[])
{
  sciPointObj * pPolyline = m_pDecomposition->getDrawedPolyline()->getDrawedObject();
  int nbVertices = m_pDecomposition->getDrawnVerticesLength();
  double * xPoints = pPOLYLINE_FEATURE(pPolyline)->pvx;
  double * xShift = pPOLYLINE_FEATURE(pPolyline)->x_shift;

  doubleArrayCopy(right, xPoints, nbVertices);
  if (xShift != NULL)
  {
    for (int i = 0; i < nbVertices; i++)
    {
      right[i] += xShift[i];
    }
  }

  // special case for logFlags
  // bars start at x = 1 = 10^0, not x = 0 since 0 can not be displayed
  char logFlags[3];
  sciGetLogFlags(sciGetParentSubwin(pPolyline), logFlags);
  if (logFlags[0] == 'l')
  {
    setDoubleArraySingleValue(left, 1.0, nbVertices);
  }
  else
  {
    setDoubleArraySingleValue(left, 0.0, nbVertices);
  }
}
/*---------------------------------------------------------------------------------*/

}
