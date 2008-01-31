/*------------------------------------------------------------------------*/
/* file: SegsArrowDrawerJavaMapper.cpp                                    */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by SegsArrowDrawer         */
/*------------------------------------------------------------------------*/

#include "SegsArrowDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
SegsArrowDrawerJavaMapper::SegsArrowDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_segsDrawing::SegsArrowDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
SegsArrowDrawerJavaMapper::~SegsArrowDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex); 
}
/*--------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::setArrowSize(double size)
{
  m_pJavaObject->setArrowSize(size);
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::setArrowSizes(const double sizes[], int nbSegments)
{
  m_pJavaObject->setArrowSizes((double *) sizes, nbSegments);
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::setAxesBounds(double xMin, double xMax,
                                              double yMin, double yMax,
                                              double zMin, double zMax)
{
  m_pJavaObject->setAxesBounds(xMin, xMax, yMin, yMax, zMin, zMax);
}
/*---------------------------------------------------------------------------------*/
void SegsArrowDrawerJavaMapper::drawSegs(const double startXCoords[], const double endXCoords[],
                                         const double startYCoords[], const double endYCoords[],
                                         const double startZCoords[], const double endZCoords[],
                                         const int colors[], int nbSegments)
{
  long * javaColors = new long[nbSegments];
  for (int i = 0; i < nbSegments; i++)
  {
    javaColors[i] = colors[i];
  }

  m_pJavaObject->drawSegs((double *)startXCoords, nbSegments,
                          (double *)endXCoords, nbSegments,
                          (double *)startYCoords, nbSegments,
                          (double *)endYCoords, nbSegments,
                          (double *)startZCoords, nbSegments,
                          (double *)endZCoords, nbSegments,
                          javaColors, nbSegments);


  delete[] javaColors;
}
/*---------------------------------------------------------------------------------*/

}
