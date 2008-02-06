/*------------------------------------------------------------------------*/
/* file: FecFacetDrawerJavaMapper.cpp                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by FecFacetDrawer          */
/*------------------------------------------------------------------------*/

#include "FecFacetDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
FecFacetDrawerJavaMapper::FecFacetDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_fecDrawing::FecFacetDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
FecFacetDrawerJavaMapper::~FecFacetDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex); 
}
/*--------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::setFacetParameters(double zMin, double zMax, int colMin,
                                                  int colMax, int colOutLow, int colOutUp)
{
  m_pJavaObject->setFacetParameters(zMin, zMax, colMin, colMax, colOutLow, colOutUp);
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJavaMapper::drawFec(const double xCoords[], const double yCoords[],
                                       const double values[], int nbNodes,
                                       const int firstPoints[], const int secondPoints[],
                                       const int thirdPoints[], int nbTriangles)
{
  // convert to long
  long * javaFirstPoints = new long[nbTriangles];
  long * javaSecondPoints = new long[nbTriangles];
  long * javaThirdPoints = new long[nbTriangles];
  for (int i = 0; i < nbTriangles; i++)
  {
    javaFirstPoints[i] = firstPoints[i];
    javaSecondPoints[i] = secondPoints[i];
    javaThirdPoints[i] = thirdPoints[i];
  }

  m_pJavaObject->drawFec((double *)xCoords, nbNodes,
                         (double *)yCoords, nbNodes,
                         (double *)values, nbNodes,
                         javaFirstPoints, nbTriangles,
                         javaSecondPoints, nbTriangles,
                         javaThirdPoints, nbTriangles);


  delete[] javaFirstPoints;
  delete[] javaSecondPoints;
  delete[] javaThirdPoints;
}
/*---------------------------------------------------------------------------------*/

}
