/*------------------------------------------------------------------------*/
/* file: InterpolatedDecomposition.cpp                                    */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Decompose polyline with creating interpolation between each     */
/*        data (this is the defautl mode)                                 */
/*------------------------------------------------------------------------*/

#include "InterpolatedDecomposition.hxx"

extern "C"
{
#include "GetProperty.h"
#include "BasicAlgos.h"
};

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
InterpolatedDecomposition::InterpolatedDecomposition( DrawablePolyline * polyline )
  : DecomposeLineStrategy(polyline)
{

}
/*------------------------------------------------------------------------------------------*/
InterpolatedDecomposition::~InterpolatedDecomposition( void )
{

}
/*------------------------------------------------------------------------------------------*/
void InterpolatedDecomposition::getDrawnVertices(double xCoords[], double yCoords[], double zCoords[])
{
  sciPointObj * pPolyline = m_pDrawed->getDrawedObject();
  int nbVertices = sciGetNbPoints(m_pDrawed->getDrawedObject());

  doubleArrayCopy(xCoords, pPOLYLINE_FEATURE(pPolyline)->pvx, nbVertices);
  doubleArrayCopy(yCoords, pPOLYLINE_FEATURE(pPolyline)->pvy, nbVertices);
  if ( pPOLYLINE_FEATURE(pPolyline)->pvz == NULL )
  {
    setDoubleArraySingleValue(zCoords, 0.0, nbVertices);
  }
  else
  {
    doubleArrayCopy(zCoords, pPOLYLINE_FEATURE(pPolyline)->pvz, nbVertices);
  }

  if (sciGetIsClosed(pPolyline))
  {
    xCoords[nbVertices] = xCoords[0];
    yCoords[nbVertices] = yCoords[0];
    zCoords[nbVertices] = zCoords[0];
  }

}
/*------------------------------------------------------------------------------------------*/
int InterpolatedDecomposition::getDrawnVerticesLength(void)
{
  int res = sciGetNbPoints(m_pDrawed->getDrawedObject());
  if (sciGetIsClosed(m_pDrawed->getDrawedObject()))
  {
    res++;
  }
  
  return res;
}
/*------------------------------------------------------------------------------------------*/

}
