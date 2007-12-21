/*------------------------------------------------------------------------*/
/* file: XGridDrawerJavaMapper.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing java methods needed by XGridDrawerJoGL         */
/*------------------------------------------------------------------------*/


#include "XGridDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
XGridDrawerJavaMapper::XGridDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::XGridDrawerGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
XGridDrawerJavaMapper::~XGridDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void XGridDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void XGridDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void XGridDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void XGridDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void XGridDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void XGridDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
void XGridDrawerJavaMapper::drawGrid(const double gridPositions[], int nbPositions)
{
  m_pJavaObject->drawGrid((double *)gridPositions, nbPositions);
}
/*--------------------------------------------------------------------------*/
void XGridDrawerJavaMapper::setGridParameters(int gridColor, float gridThickness)
{
  m_pJavaObject->setGridParameters(gridColor, gridThickness);
}
/*--------------------------------------------------------------------------*/
void XGridDrawerJavaMapper::setAxesBounds(double xMin, double xMax,
                                          double yMin, double yMax,
                                          double zMin, double zMax)
{
  m_pJavaObject->setAxesBounds(xMin, xMax, yMin, yMax, zMin, zMax);
}
/*--------------------------------------------------------------------------*/
}

