/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableSurface.cpp                                      */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing implementation of driver independent           */
/*        algorithms                                                      */
/*------------------------------------------------------------------------*/

#include "ConcreteDrawableSurface.hxx"

namespace sciGraphics
{

using namespace std;

/*---------------------------------------------------------------------------------*/
ConcreteDrawableSurface::ConcreteDrawableSurface(sciPointObj * pSurface)
  : DrawableSurface(pSurface)
{
  m_oDrawingStrategies.clear();
}
/*---------------------------------------------------------------------------------*/
ConcreteDrawableSurface::~ConcreteDrawableSurface(void)
{
  removeDrawingStrategies();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableSurface::addDrawingStrategy(DrawSurfaceStrategy * strategy)
{
  m_oDrawingStrategies.push_back(strategy);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableSurface::removeDrawingStrategies(void)
{
  list<DrawSurfaceStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    delete *it;
    *it = NULL;
  }
  m_oDrawingStrategies.clear();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableSurface::drawSurface(void)
{
  list<DrawSurfaceStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for(; it != m_oDrawingStrategies.end(); it++)
  {
    (*it)->drawSurface();
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableSurface::showSurface(void)
{
  list<DrawSurfaceStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for(; it != m_oDrawingStrategies.end(); it++)
  {
    (*it)->showSurface();
  }
}
/*---------------------------------------------------------------------------------*/

}
