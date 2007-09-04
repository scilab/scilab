/*------------------------------------------------------------------------*/
/* file: ConcreteDrawablePolyline.cpp                                     */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing implementation of driver independent           */
/*        algorithms                                                      */
/*------------------------------------------------------------------------*/

#include "ConcreteDrawablePolyline.hxx"

namespace sciGraphics
{

using namespace std;

/*------------------------------------------------------------------------------------------*/
ConcreteDrawablePolyline::ConcreteDrawablePolyline(sciPointObj * pObj)
  : DrawablePolyline(pObj)
{
  m_oDrawingStrategies.clear();
  m_pDecomposeStrategy = NULL;
}
/*------------------------------------------------------------------------------------------*/
ConcreteDrawablePolyline::~ConcreteDrawablePolyline(void)
{
   removeDrawingStrategies();
   removeDecompositionStrategy();
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawablePolyline::addDrawingStrategy(DrawPolylineStrategy * strategy)
{
  m_oDrawingStrategies.push_back(strategy);
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawablePolyline::removeDrawingStrategies(void)
{
  list<DrawPolylineStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    delete *it;
    *it = NULL;
  }
  m_oDrawingStrategies.clear();
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawablePolyline::setDecompositionStrategy(DecomposeLineStrategy * strategy)
{
  m_pDecomposeStrategy = strategy;
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawablePolyline::removeDecompositionStrategy(void)
{
  delete m_pDecomposeStrategy;
  m_pDecomposeStrategy = NULL;
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawablePolyline::getDrawnVertices(double xCoords[], double yCcoords[], double zCcoords[])
{
  m_pDecomposeStrategy->getDrawnVertices(xCoords, yCcoords, zCcoords);
}
/*------------------------------------------------------------------------------------------*/
int ConcreteDrawablePolyline::getDrawnVerticesLength(void)
{
  return m_pDecomposeStrategy->getDrawnVerticesLength();
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawablePolyline::drawPolyline(void)
{
  list<DrawPolylineStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->drawPolyline();
  }
}
/*------------------------------------------------------------------------------------------*/
void ConcreteDrawablePolyline::showPolyline(void)
{
  list<DrawPolylineStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->showPolyline();
  }
}
/*------------------------------------------------------------------------------------------*/

}
