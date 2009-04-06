/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Decompose polyline to get data for bar drawing
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "BarDecomposition.hxx"
#include "HorizontalBarDecomposition.hxx"
#include "VerticalBarDecomposition.hxx"

extern "C"
{
#include "GetProperty.h"
#include "BasicAlgos.h"
};

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
BarDecomposition::BarDecomposition( DrawablePolyline * polyline )
  : DecomposeLineStrategy(polyline)
{
  sciPointObj * pPolyline = polyline->getDrawedObject();
  if (sciGetPolylineStyle(pPolyline) == 7)
  {
    m_pExtentComputer = new HorizontalBarDecomposition(this);
  }
  else
  {
    m_pExtentComputer = new VerticalBarDecomposition(this);
  }
}
/*---------------------------------------------------------------------------------*/
BarDecomposition::~BarDecomposition( void )
{
  delete m_pExtentComputer;
  m_pExtentComputer = NULL;
}
/*---------------------------------------------------------------------------------*/
void BarDecomposition::getDrawnVertices(double xCoords[], double yCoords[], double zCoords[])
{
  sciPointObj * pPolyline = m_pDrawed->getDrawedObject();
  int nbVertices = sciGetNbPoints(m_pDrawed->getDrawedObject());
  double * xPoints = pPOLYLINE_FEATURE(pPolyline)->pvx;
  double * yPoints = pPOLYLINE_FEATURE(pPolyline)->pvy;
  double * zPoints = pPOLYLINE_FEATURE(pPolyline)->pvz;
  double * xShift = pPOLYLINE_FEATURE(pPolyline)->x_shift;
  double * yShift = pPOLYLINE_FEATURE(pPolyline)->y_shift;
  double * zShift = pPOLYLINE_FEATURE(pPolyline)->z_shift;

  if (xCoords != NULL)
  {
    doubleArrayCopy(xCoords, xPoints, nbVertices);
    if (xShift != NULL)
    {
      for(int i = 0; i < nbVertices; i++)
      {
        xCoords[i] += xShift[i];
      }
    }
  }
  

  if (yCoords != NULL)
  {
    doubleArrayCopy(yCoords, yPoints, nbVertices);
    if (yShift != NULL)
    {
      for(int i = 0; i < nbVertices; i++)
      {
        yCoords[i] += yShift[i];
      }
    }
  }
  
  if (zCoords != NULL)
  {
    if ( zPoints == NULL )
    {
      setDoubleArraySingleValue(zCoords, 0.0, nbVertices);
    }
    else
    {
      doubleArrayCopy(zCoords, zPoints, nbVertices);
    }

    if (zShift != NULL)
    {
      for(int i = 0; i < nbVertices; i++)
      {
        zCoords[i] += zShift[i];
      }
    }
  }
  

  // apply logarithmic mode if needed
  m_pDrawed->pointScale(xCoords, yCoords, zCoords, getDrawnVerticesLength());
  
}
/*---------------------------------------------------------------------------------*/
void BarDecomposition::getDrawnVerticesColor(int colors[])
{
  // unused for now
  for(int i = 0; i < getDrawnVerticesLength(); i++)
  {
    colors[i] = 0;
  }
}
/*---------------------------------------------------------------------------------*/
int BarDecomposition::getDrawnVerticesLength(void)
{
  // can't be closed
  return sciGetNbPoints(m_pDrawed->getDrawedObject());
}
/*---------------------------------------------------------------------------------*/
void BarDecomposition::getBarOrdinates(double bottom[], double top[])
{

  int nbVertices = getDrawnVerticesLength();
  getBarLinearOrdinates(bottom, top);

  m_pDrawed->pointScale(NULL, bottom, NULL, nbVertices);
  m_pDrawed->pointScale(NULL, top, NULL, nbVertices);
}
/*---------------------------------------------------------------------------------*/
void BarDecomposition::getBarAbscissas(double left[], double right[])
{
  // apply log scale if needed
  int nbVertices = getDrawnVerticesLength();
  getBarLinearAbscissas(left, right);

  m_pDrawed->pointScale(left, NULL, NULL, nbVertices);
  m_pDrawed->pointScale(right, NULL, NULL, nbVertices);

}
/*---------------------------------------------------------------------------------*/
void BarDecomposition::getBarPlotMarkVertices(double xCoords[], double yCoords[], double zCoords[])
{
  // first half
  getDrawnVertices(xCoords, yCoords, zCoords);

  int semiSize = getDrawnVerticesLength();

  // second half
  sciPointObj * pPolyline = m_pDrawed->getDrawedObject();
  
  // special case for with logarithmic mode
  // bars starts at y = 1 = 10^0 and not 0 which is not displayable
  char logFlags[3];
  sciGetLogFlags(sciGetParentSubwin(pPolyline), logFlags);
  if (logFlags[1] == 'l')
  {
    for (int i = 0; i < semiSize ; i++)
    {
      xCoords[i + semiSize] = xCoords[i];
      yCoords[i + semiSize] = 1.0;
      zCoords[i + semiSize] = zCoords[i];
    }
  }
  else
  {
    for (int i = 0; i < semiSize ; i++)
    {
      xCoords[i + semiSize] = xCoords[i];
      yCoords[i + semiSize] = 0.0;
      zCoords[i + semiSize] = zCoords[i];
    }
  }
  
}
/*---------------------------------------------------------------------------------*/
int BarDecomposition::getBarPlotMarkVerticesLength(void)
{
  return 2 * getDrawnVerticesLength();
}
/*---------------------------------------------------------------------------------*/
void BarDecomposition::getBarLinearOrdinates(double bottom[], double top[])
{
  m_pExtentComputer->getBarOrdinates(bottom, top);
}
/*---------------------------------------------------------------------------------*/
void BarDecomposition::getBarLinearAbscissas(double left[], double right[])
{
  m_pExtentComputer->getBarAbscissas(left, right);
}
/*---------------------------------------------------------------------------------*/
}
