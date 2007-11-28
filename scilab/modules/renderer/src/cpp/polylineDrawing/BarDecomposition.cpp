/*------------------------------------------------------------------------*/
/* file: StairCaseDecomposition.cpp                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Decompose polyline to get data for bar drawing                  */
/*------------------------------------------------------------------------*/


#include "BarDecomposition.hxx"

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

}
/*---------------------------------------------------------------------------------*/
BarDecomposition::~BarDecomposition( void )
{

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
void BarDecomposition::getBarHeight(double heights[])
{
  sciPointObj * pPolyline = m_pDrawed->getDrawedObject();
  int nbVertices = sciGetNbPoints(pPolyline);
  double * yPoints = pPOLYLINE_FEATURE(pPolyline)->pvy;
  double * yShift = pPOLYLINE_FEATURE(pPolyline)->y_shift;

  doubleArrayCopy(heights, yPoints, nbVertices);

  if (sciGetPolylineStyle(pPolyline) == 3 && yShift != NULL)
  {
    // barplot, bars start at y = 0
    for (int i = 0; i < nbVertices; i++)
    {
      heights[i] += yShift[i];
    }
    // now consider that ySHift is NULL.
    yShift = NULL;
  }

  // special case for logFlags
  // bars start at y = 1 = 10^0, not y = 0 since 0 can not be displayed
  char logFlags[3];
  sciGetLogFlags(sciGetParentSubwin(pPolyline), logFlags);
  if (logFlags[1] == 'l')
  {
    for (int i = 0; i < nbVertices; i++)
    {
      heights[i] -= 1.0;
    }
  }

  
  // apply log scale if needed
  if (yShift == NULL)
  {
    for (int i = 0; i < nbVertices; i++)
    {
      double dummyX = 1.0;
      double dummyZ = 1.0;
      m_pDrawed->directionScale(dummyX, heights[i], dummyZ,
                                dummyX, 1.0, dummyZ,
                                &dummyX, &(heights[i]), &dummyZ);
    }
  }
  else
  {
    for (int i = 0; i < nbVertices; i++)
    {
      double dummyX = 1.0;
      double dummyZ = 1.0;
      m_pDrawed->directionScale(dummyX, heights[i], dummyZ,
                                dummyX, 1.0 + yShift[i], dummyZ,
                                &dummyX, &(heights[i]), &dummyZ);
    }
  }
}
/*---------------------------------------------------------------------------------*/
void BarDecomposition::getBarWidth(double left[], double right[])
{
  sciPointObj * pPolyline = m_pDrawed->getDrawedObject();
  int nbVertices = getDrawnVerticesLength();
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

  // apply logarithmic mode if needed
  for (int i = 0; i < nbVertices; i++)
  {
    double dummyY = 1.0;
    double dummyZ = 1.0;
    m_pDrawed->pointScale(xPoints[i] - barWidth / 2.0, dummyY, dummyZ,
                          &(left[i]),&dummyY, &dummyZ);
    m_pDrawed->pointScale(xPoints[i] + barWidth / 2.0, dummyY, dummyZ,
                          &(right[i]),&dummyY, &dummyZ);
  }

}
/*---------------------------------------------------------------------------------*/
void BarDecomposition::getBarPlotMarkVertices(double xCoords[], double yCoords[], double zCoords[])
{
  // first half
  getDrawnVertices(xCoords, yCoords, zCoords);

  int semiSize = getDrawnVerticesLength();

  // second half
  for (int i = 0; i < semiSize ; i++)
  {
    xCoords[i + semiSize] = xCoords[i];
    yCoords[i + semiSize] = 0.0;
    zCoords[i + semiSize] = zCoords[i];
  }
}
/*---------------------------------------------------------------------------------*/
int BarDecomposition::getBarPlotMarkVerticesLength(void)
{
  return 2 * getDrawnVerticesLength();
}
/*---------------------------------------------------------------------------------*/

}
