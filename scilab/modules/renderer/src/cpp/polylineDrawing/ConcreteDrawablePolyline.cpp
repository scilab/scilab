/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing implementation of driver independent algorithms
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ConcreteDrawablePolyline.hxx"

namespace sciGraphics
{

using namespace std;

/*---------------------------------------------------------------------------------*/
ConcreteDrawablePolyline::ConcreteDrawablePolyline(sciPointObj * pObj)
  : DrawablePolyline(pObj)
{
  m_oDrawingStrategies.clear();
  m_pDecomposeStrategy = NULL;
}
/*---------------------------------------------------------------------------------*/
ConcreteDrawablePolyline::~ConcreteDrawablePolyline(void)
{
   removeDrawingStrategies();
   setDecompositionStrategy(NULL);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawablePolyline::addDrawingStrategy(DrawPolylineStrategy * strategy)
{
  m_oDrawingStrategies.push_back(strategy);
}
/*---------------------------------------------------------------------------------*/
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
/*---------------------------------------------------------------------------------*/
void ConcreteDrawablePolyline::setDecompositionStrategy(DecomposeLineStrategy * strategy)
{
  if (m_pDecomposeStrategy != NULL)
  {
    delete m_pDecomposeStrategy;
  }
  m_pDecomposeStrategy = strategy;
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawablePolyline::getDrawnVertices(double xCoords[], double yCcoords[], double zCcoords[])
{
  m_pDecomposeStrategy->getDrawnVertices(xCoords, yCcoords, zCcoords);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawablePolyline::getDrawnVerticesColor(int colors[])
{
  m_pDecomposeStrategy->getDrawnVerticesColor(colors);
}
/*---------------------------------------------------------------------------------*/
int ConcreteDrawablePolyline::getDrawnVerticesLength(void)
{
  return m_pDecomposeStrategy->getDrawnVerticesLength();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawablePolyline::drawPolyline(void)
{
  list<DrawPolylineStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->drawPolyline();
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawablePolyline::showPolyline(void)
{
  list<DrawPolylineStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->showPolyline();
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawablePolyline::redrawPolyline(void)
{
  list<DrawPolylineStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for( ; it != m_oDrawingStrategies.end(); it++ )
  {
    (*it)->redrawPolyline();
  }
}
/*---------------------------------------------------------------------------------*/
}
