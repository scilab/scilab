/*------------------------------------------------------------------------*/
/* file: SurfaceFacetDrawerJavaMapper.cpp                                 */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by SurfaceFacetDrawerJoGL  */
/*------------------------------------------------------------------------*/

#include "SurfaceFacetDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
SurfaceFacetDrawerJavaMapper::SurfaceFacetDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_surfaceDrawing::SurfaceFacetDrawerGL(getScilabJavaVM());
}
/*---------------------------------------------------------------------------------*/
SurfaceFacetDrawerJavaMapper::~SurfaceFacetDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*---------------------------------------------------------------------------------*/
void SurfaceFacetDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*---------------------------------------------------------------------------------*/
void SurfaceFacetDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SurfaceFacetDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*---------------------------------------------------------------------------------*/
void SurfaceFacetDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SurfaceFacetDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SurfaceFacetDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*---------------------------------------------------------------------------------*/
void SurfaceFacetDrawerJavaMapper::setDefaultColors(int defaultColor, int hiddenColor)
{
  m_pJavaObject->setDefaultColors(defaultColor, hiddenColor);
}
/*---------------------------------------------------------------------------------*/
void SurfaceFacetDrawerJavaMapper::setSurfaceType(int typeOfSurface, int colorFlag)
{
  m_pJavaObject->setSurfaceType(typeOfSurface, colorFlag);
}
/*---------------------------------------------------------------------------------*/
void SurfaceFacetDrawerJavaMapper::drawSurface(double xCoords[], int sizeX,
                                               double yCoords[], int sizeY,
                                               double zCoords[], int sizeZ,
                                               int nbFacets)
{
  m_pJavaObject->drawSurface(xCoords, sizeX, yCoords, sizeY, zCoords, sizeZ, nbFacets);
}
/*---------------------------------------------------------------------------------*/
void SurfaceFacetDrawerJavaMapper::setFacetsColors(double colors[], int colorsSize)
{

  m_pJavaObject->setFacetsColors(colors, colorsSize);
}
/*---------------------------------------------------------------------------------*/
}
