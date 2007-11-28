/*------------------------------------------------------------------------*/
/* file: PolylineBarDrawerJavaMapper.cpp                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of Java method used by PolylineBarDrawerJoGL   */
/*------------------------------------------------------------------------*/


#include "PolylineBarDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
PolylineBarDrawerJavaMapper::PolylineBarDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_polylineDrawing::PolylineBarDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
PolylineBarDrawerJavaMapper::~PolylineBarDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::setBarParameters(int background, int foreground, float thickness,
                                                   int lineStyle)
{
  m_pJavaObject->setBarParameters(background, foreground, thickness, lineStyle);
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJavaMapper::drawPolyline(const double topX[], const double topY[],
                                               const double topZ[], const double height[],
                                               const double left[], const double right[], int nbCoords)
{
  m_pJavaObject->drawPolyline((double *)topX, nbCoords,
                              (double *)topY, nbCoords,
                              (double *)topZ, nbCoords,
                              (double *)height, nbCoords,
                              (double *)left, nbCoords,
                              (double *)right, nbCoords); 
}
/*---------------------------------------------------------------------------------*/
}
