/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Decompose polyline with creating interpolation between each
 *  data (this is the defautl mode)
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "InterpolatedDecomposition.hxx"

extern "C"
{
#include "GetProperty.h"
#include "BasicAlgos.h"
};

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
InterpolatedDecomposition::InterpolatedDecomposition( DrawablePolyline * polyline )
  : DecomposeLineStrategy(polyline)
{

}
/*---------------------------------------------------------------------------------*/
InterpolatedDecomposition::~InterpolatedDecomposition( void )
{

}
/*---------------------------------------------------------------------------------*/
void InterpolatedDecomposition::getDrawnVertices(double xCoords[], double yCoords[], double zCoords[])
{
  sciPointObj * pPolyline = m_pDrawed->getDrawedObject();
  int nbVertices = sciGetNbPoints(m_pDrawed->getDrawedObject());
  double * xPoints = pPOLYLINE_FEATURE(pPolyline)->pvx;
  double * yPoints = pPOLYLINE_FEATURE(pPolyline)->pvy;
  double * zPoints = pPOLYLINE_FEATURE(pPolyline)->pvz;
  double * xShift = pPOLYLINE_FEATURE(pPolyline)->x_shift;
  double * yShift = pPOLYLINE_FEATURE(pPolyline)->y_shift;
  double * zShift = pPOLYLINE_FEATURE(pPolyline)->z_shift;

  doubleArrayCopy(xCoords, xPoints, nbVertices);
  doubleArrayCopy(yCoords, yPoints, nbVertices);
  
  if ( zPoints == NULL )
  {
    setDoubleArraySingleValue(zCoords, 0.0, nbVertices);
  }
  else
  {
    doubleArrayCopy(zCoords, zPoints, nbVertices);
  }

  if (xShift != NULL)
  {
    for(int i = 0; i < nbVertices; i++)
    {
      xCoords[i] += xShift[i];
    }
  }

  if (yShift != NULL)
  {
    for(int i = 0; i < nbVertices; i++)
    {
      yCoords[i] += yShift[i];
    }
  }

  if (zShift != NULL)
  {
    for(int i = 0; i < nbVertices; i++)
    {
      zCoords[i] += zShift[i];
    }
  }

  if (sciGetIsClosed(pPolyline))
  {
    xCoords[nbVertices] = xCoords[0];
    yCoords[nbVertices] = yCoords[0];
    zCoords[nbVertices] = zCoords[0];
  }

  // apply logarithmic mode if needed
  m_pDrawed->pointScale(xCoords, yCoords, zCoords, getDrawnVerticesLength());

}
/*---------------------------------------------------------------------------------*/
void InterpolatedDecomposition::getDrawnVerticesColor(int colors[])
{
  sciPointObj * pPolyline = m_pDrawed->getDrawedObject();
  int nbVertices = sciGetNbPoints(pPolyline);
  int * interpColors = sciGetInterpVector(pPolyline);
  
  intArrayCopy(colors, interpColors, nbVertices);
  
  if (sciGetIsClosed(pPolyline))
  {
    colors[nbVertices] = colors[0];
  }
}
/*---------------------------------------------------------------------------------*/
int InterpolatedDecomposition::getDrawnVerticesLength(void)
{
  int res = sciGetNbPoints(m_pDrawed->getDrawedObject());
  if (sciGetIsClosed(m_pDrawed->getDrawedObject()))
  {
    res++;
  }
  
  return res;
}
/*---------------------------------------------------------------------------------*/

}
