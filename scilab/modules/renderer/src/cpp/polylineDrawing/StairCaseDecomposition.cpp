/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Decompose polyline with creating a stair case between each data
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "StairCaseDecomposition.hxx"

extern "C"
{
#include "GetProperty.h"
#include "BasicAlgos.h"
};

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
StairCaseDecomposition::StairCaseDecomposition( DrawablePolyline * polyline )
  : DecomposeLineStrategy(polyline)
{

}
/*---------------------------------------------------------------------------------*/
StairCaseDecomposition::~StairCaseDecomposition( void )
{

}
/*---------------------------------------------------------------------------------*/
void StairCaseDecomposition::getDrawnVertices(double xCoords[], double yCoords[], double zCoords[])
{
  sciPointObj * pPolyline = m_pDrawed->getDrawedObject();
  int nbPolylinePoints = sciGetNbPoints(pPolyline); /* number of points specified by the user */
  int nbVertices = 2 * nbPolylinePoints - 1;
  double * xPoints = pPOLYLINE_FEATURE(pPolyline)->pvx;
  double * yPoints = pPOLYLINE_FEATURE(pPolyline)->pvy;
  double * zPoints = pPOLYLINE_FEATURE(pPolyline)->pvz;

  if ( zPoints == NULL )
  {
    for ( int i = 0 ; i < nbPolylinePoints - 1 ; i++ )
    {
      xCoords[2 * i] = xPoints[i];
      yCoords[2 * i] = yPoints[i];
      
      xCoords[2 * i + 1] = xPoints[i + 1];
      yCoords[2 * i + 1] = yPoints[i];
    }

    // the last point is the same for the two curves
    xCoords[nbVertices - 1] = xPoints[nbPolylinePoints - 1];
    yCoords[nbVertices - 1] = yPoints[nbPolylinePoints - 1];

    setDoubleArraySingleValue(zCoords, 0.0, nbVertices);
  }
  else
  {
    for ( int i = 0 ; i < nbPolylinePoints - 1 ; i++ )
    {
      xCoords[2 * i] = xPoints[i];
      yCoords[2 * i] = yPoints[i];
      zCoords[2 * i] = zPoints[i];

      xCoords[2 * i + 1] = xPoints[i + 1];
      yCoords[2 * i + 1] = yPoints[i];
      zCoords[2 * i + 1] = zPoints[i];
    }

    // the last point is the same for the two curves
    xCoords[nbVertices - 1] = xPoints[nbPolylinePoints - 1];
    yCoords[nbVertices - 1] = yPoints[nbPolylinePoints - 1];
    zCoords[nbVertices - 1] = zPoints[nbPolylinePoints - 1];
  }

  double * xShift = pPOLYLINE_FEATURE(pPolyline)->x_shift;
  double * yShift = pPOLYLINE_FEATURE(pPolyline)->y_shift;
  double * zShift = pPOLYLINE_FEATURE(pPolyline)->z_shift;

  // add shifting to all vertices
  if (xShift != NULL)
  {
    for(int i = 0; i < nbPolylinePoints - 1; i++)
    {
      xCoords[2 * i] += xShift[i];
      xCoords[2 * i + 1] += xShift[i + 1];
    }
    xCoords[nbVertices - 1] += xShift[nbPolylinePoints - 1];
  }

  if (yShift != NULL)
  {
    for(int i = 0; i < nbPolylinePoints - 1; i++)
    {
      yCoords[2 * i] += yShift[i];
      yCoords[2 * i + 1] += yShift[i];
    }
    yCoords[nbVertices - 1] += yShift[nbPolylinePoints - 1];
  }

  if (zShift != NULL)
  {
    for(int i = 0; i < nbPolylinePoints - 1; i++)
    {
      zCoords[2 * i] += zShift[i];
      zCoords[2 * i + 1] += zShift[i];
    }
    zCoords[nbVertices - 1] += zShift[nbPolylinePoints - 1];
  }

  // one more stair case
  if (sciGetIsClosed(pPolyline))
  {
    xCoords[nbVertices] = xCoords[0];
    yCoords[nbVertices] = yCoords[nbVertices - 1];
    zCoords[nbVertices] = zCoords[nbVertices - 1];

    xCoords[nbVertices + 1] = xCoords[0];
    yCoords[nbVertices + 1] = yCoords[0];
    zCoords[nbVertices + 1] = zCoords[0];
  }

  // apply logarithmic mode if needed
  m_pDrawed->pointScale(xCoords, yCoords, zCoords, getDrawnVerticesLength());

}
/*---------------------------------------------------------------------------------*/
void StairCaseDecomposition::getDrawnVerticesColor(int colors[])
{
  sciPointObj * pPolyline = m_pDrawed->getDrawedObject();
  int nbPoints = sciGetNbPoints(pPolyline);
  int nbVertices = 2 * nbPoints - 1;
  int * interpColors = sciGetInterpVector(pPolyline);

  for (int i = 0; i < nbPoints - 1; i++ )
  {
    colors[2 * i    ] = interpColors[i];
    colors[2 * i + 1] = interpColors[i];
  }
  colors[nbVertices - 1] = interpColors[nbPoints - 1];

  // one more stair case
  if (sciGetIsClosed(pPolyline))
  {
    colors[nbVertices]     = colors[0];
    colors[nbVertices + 1] = colors[0];
  }
}
/*---------------------------------------------------------------------------------*/
int StairCaseDecomposition::getDrawnVerticesLength(void)
{
  int res = 2 * sciGetNbPoints(m_pDrawed->getDrawedObject()) - 1;

  // one more stair case
  if (sciGetIsClosed(m_pDrawed->getDrawedObject()))
  {
    res += 2;
  }

  return res;
}
/*---------------------------------------------------------------------------------*/

}
