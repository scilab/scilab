/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Decompose polyline to get data for vertical bar drawing
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "VerticalBarDecomposition.hxx"

extern "C"
{
#include "BasicAlgos.h"
#include "GetProperty.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
VerticalBarDecomposition::VerticalBarDecomposition( BarDecomposition * decomposition )
  : LinearBarDecomposition(decomposition)
{

}
/*---------------------------------------------------------------------------------*/
VerticalBarDecomposition::~VerticalBarDecomposition( void )
{

}
/*---------------------------------------------------------------------------------*/
void VerticalBarDecomposition::getBarOrdinates(double bottom[], double top[])
{
  sciPointObj * pPolyline = m_pDecomposition->getDrawedPolyline()->getDrawedObject();
  int nbVertices = m_pDecomposition->getDrawnVerticesLength();
  double * yPoints = pPOLYLINE_FEATURE(pPolyline)->pvy;
  double * yShift = pPOLYLINE_FEATURE(pPolyline)->y_shift;

  doubleArrayCopy(top, yPoints, nbVertices);


  // special case for logFlags
  // bars start at y = 1 = 10^0, not y = 0 since 0 can not be displayed
  char logFlags[3];
  sciGetLogFlags(sciGetParentSubwin(pPolyline), logFlags);
  if (logFlags[1] == 'l')
  {
    setDoubleArraySingleValue(bottom, 1.0, nbVertices);
  }
  else
  {
    setDoubleArraySingleValue(bottom, 0.0, nbVertices);
  }


  if (yShift != NULL)
  {
    for (int i = 0; i < nbVertices; i++)
    {
      top[i] += yShift[i];
    }
  }   

  // for barplot, bars always start at y = 0
  if (sciGetPolylineStyle(pPolyline) != 3 && yShift != NULL)
  {
    for (int i = 0; i < nbVertices; i++)
    {
      bottom[i] +=  yShift[i];
    }
  }

}
/*---------------------------------------------------------------------------------*/
void VerticalBarDecomposition::getBarAbscissas(double left[], double right[])
{
  sciPointObj * pPolyline = m_pDecomposition->getDrawedPolyline()->getDrawedObject();
  int nbVertices = m_pDecomposition->getDrawnVerticesLength();
  double * xPoints = pPOLYLINE_FEATURE(pPolyline)->pvx;
  double * xShift = pPOLYLINE_FEATURE(pPolyline)->x_shift;

  double barWidth;
  if (sciGetPolylineStyle(pPolyline) == 3)
  {
    // bar plot mode
    barWidth = 0.0;
  }
  else
  {
    barWidth = pPOLYLINE_FEATURE(pPolyline)->bar_width;
  }

  for (int i = 0; i < nbVertices; i++)
  {
    left[i] = xPoints[i] - barWidth / 2.0;
    right[i] = xPoints[i] + barWidth / 2.0;
  }

  if (xShift != NULL)
  {
    for (int i = 0; i < nbVertices; i++)
    {
      left[i] += xShift[i];
      right[i] += xShift[i];
    }
  }
}
/*---------------------------------------------------------------------------------*/

}

