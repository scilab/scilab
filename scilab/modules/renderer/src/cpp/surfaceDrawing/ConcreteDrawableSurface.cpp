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
void ConcreteDrawableSurface::redrawSurface(void)
{
  list<DrawSurfaceStrategy *>::iterator it = m_oDrawingStrategies.begin();
  for(; it != m_oDrawingStrategies.end(); it++)
  {
    (*it)->redrawSurface();
  }
}
/*---------------------------------------------------------------------------------*/
}
