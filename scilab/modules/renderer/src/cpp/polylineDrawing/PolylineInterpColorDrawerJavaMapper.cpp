/*------------------------------------------------------------------------*/
/* file: PolylineInterpColorDrawerJavaMapper.cpp                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains mapping of java method used by PolylineLineDrawer      */
/*------------------------------------------------------------------------*/

#include "PolylineInterpColorDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
PolylineInterpColorDrawerJavaMapper::PolylineInterpColorDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_polylineDrawing::PolylineInterpColorDrawerGL(getScilabJavaVM());
}
/*------------------------------------------------------------------------------------------*/
PolylineInterpColorDrawerJavaMapper::~PolylineInterpColorDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*------------------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*------------------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJavaMapper::destroy(int figureIndex)
{
  m_pJavaObject->destroy(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*------------------------------------------------------------------------------------------*/
void PolylineInterpColorDrawerJavaMapper::drawPolyline(const double xCoords[], const double yCoords[],
                                                       const double zCoords[], const int colors[], int nbCoords)
{
  // temporary, convert int * to long *
  long * colorsLong = new long[nbCoords];
  for ( int i = 0 ; i < nbCoords ; i++ )
  {
    colorsLong[i] = colors[i];
  }
  m_pJavaObject->drawPolyline((double *)xCoords, nbCoords,
                              (double *)yCoords, nbCoords,
                              (double *)zCoords, nbCoords,
                              colorsLong,  nbCoords);

  delete[] colorsLong;
}
/*------------------------------------------------------------------------------------------*/
}
