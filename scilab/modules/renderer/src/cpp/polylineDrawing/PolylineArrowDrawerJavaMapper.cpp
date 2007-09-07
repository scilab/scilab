/*------------------------------------------------------------------------*/
/* file: PolylineArrowDrawerJavaMapper.cpp                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of Java methods used by RectangleArrowDrawer   */
/*------------------------------------------------------------------------*/


#include "PolylineArrowDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
PolylineArrowDrawerJavaMapper::PolylineArrowDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_polylineDrawing::PolylineArrowDrawerGL(getScilabJavaVM());
}
/*------------------------------------------------------------------------------------------*/
PolylineArrowDrawerJavaMapper::~PolylineArrowDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*------------------------------------------------------------------------------------------*/
void PolylineArrowDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*------------------------------------------------------------------------------------------*/
void PolylineArrowDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void PolylineArrowDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void PolylineArrowDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void PolylineArrowDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void PolylineArrowDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void PolylineArrowDrawerJavaMapper::setArrowParameters(int arrowColor, double arrowSize)
{
  m_pJavaObject->setArrowParameters(arrowColor, arrowSize);
}
/*------------------------------------------------------------------------------------------*/
void PolylineArrowDrawerJavaMapper::setAxesBounds(double xMin, double xMax,
                                                  double yMin, double yMax,
                                                  double zMin, double zMax)
{
  m_pJavaObject->setAxesBounds(xMin, xMax, yMin, yMax, zMin, zMax);
}
/*------------------------------------------------------------------------------------------*/
void PolylineArrowDrawerJavaMapper::drawPolyline(const double xCoords[], const double yCoords[], const double zCoords[], int nbCoords)
{
  m_pJavaObject->drawPolyline((double *)xCoords, nbCoords,
                                (double *)yCoords, nbCoords,
                                (double *)zCoords, nbCoords);
}
/*------------------------------------------------------------------------------------------*/
}
