/*------------------------------------------------------------------------*/
/* file: SegsLineDrawerJavaMapper.cpp                                     */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by SegsLineDrawer          */
/*------------------------------------------------------------------------*/

#include "SegsLineDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
SegsLineDrawerJavaMapper::SegsLineDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_segsDrawing::SegsLineDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
SegsLineDrawerJavaMapper::~SegsLineDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::destroy(int parentFigureIndex)
{
 m_pJavaObject->destroy(parentFigureIndex); 
}
/*--------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::setLineParameters(float thickness, int lineStyle)
{
  m_pJavaObject->setLineParameters(thickness, lineStyle);
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJavaMapper::drawSegs(const double startXCoords[], const double endXCoords[],
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
