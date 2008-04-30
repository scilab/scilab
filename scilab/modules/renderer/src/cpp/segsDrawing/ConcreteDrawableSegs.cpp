/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing implementation of driver independent
 * algorithms of drawable segs
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "ConcreteDrawableSegs.hxx"

namespace sciGraphics
{

using namespace std;

/*---------------------------------------------------------------------------------*/
ConcreteDrawableSegs::ConcreteDrawableSegs(sciPointObj * pSegs)
  : DrawableSegs(pSegs)
{
  m_oDrawers.clear();
  m_pDecomposer = NULL;
}
/*---------------------------------------------------------------------------------*/
ConcreteDrawableSegs::~ConcreteDrawableSegs(void)
{
  removeDrawingStrategies();
  setDecomposeStrategy(NULL);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableSegs::getSegsPos(double startXcoords[], double endXCoords[],
                                      double startYCoords[], double endYCoords[],
                                      double startZCoords[], double endZcoords[])
{
  m_pDecomposer->getSegsPos(startXcoords, endXCoords, startYCoords,
                            endYCoords, startZCoords, endZcoords);
}
/*---------------------------------------------------------------------------------*/
int ConcreteDrawableSegs::getNbSegment(void)
{
  return m_pDecomposer->getNbSegment();
}
/*---------------------------------------------------------------------------------*/
bool ConcreteDrawableSegs::isColored(void)
{
  return m_pDecomposer->isColored();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableSegs::getSegsColors(int colors[])
{
  m_pDecomposer->getSegsColors(colors);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableSegs::setDecomposeStrategy(DecomposeSegsStrategy * decomposer)
{
  if (m_pDecomposer != NULL)
  {
    delete m_pDecomposer;
  }
  m_pDecomposer = decomposer;
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableSegs::addDrawingStrategy(DrawSegsStrategy * drawer)
{
  m_oDrawers.push_back(drawer);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableSegs::removeDrawingStrategies(void)
{
  list<DrawSegsStrategy *>::iterator it = m_oDrawers.begin();
  for( ; it != m_oDrawers.end(); it++ )
  {
    delete *it;
    *it = NULL;
  }
  m_oDrawers.clear();
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableSegs::getBoundingBox(double bounds[6])
{
  m_pDecomposer->getBoundingBox(bounds);
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableSegs::drawSegs(void)
{
  int nbSegs = getNbSegment();
  double * xStarts = new double[nbSegs];
  double * xEnds   = new double[nbSegs];
  double * yStarts = new double[nbSegs];
  double * yEnds   = new double[nbSegs];
  double * zStarts = new double[nbSegs];
  double * zEnds   = new double[nbSegs];
  int * colors = NULL;

  getSegsPos(xStarts, xEnds, yStarts, yEnds, zStarts, zEnds);

  if (isColored())
  {
    colors  = new int[nbSegs];
    getSegsColors(colors);
  }

  drawSegs(xStarts, xEnds, yStarts, yEnds, zStarts, zEnds, colors, nbSegs);

  delete[] xStarts;
  delete[] xEnds;
  delete[] yStarts;
  delete[] yEnds;
  delete[] zStarts;
  delete[] zEnds;
  if (isColored())
  {
    delete[] colors;
  }
  


}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableSegs::drawSegs(const double xStarts[], const double xEnds[],
                                    const double yStarts[], const double yEnds[],
                                    const double zStarts[], const double zEnds[],
                                    const int colors[], int nbSegment)
{
  list<DrawSegsStrategy *>::iterator it = m_oDrawers.begin();
  for( ; it != m_oDrawers.end(); it++ )
  {
    (*it)->drawSegs(xStarts, xEnds, yStarts, yEnds, zStarts, zEnds, colors, nbSegment);
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableSegs::showSegs(void)
{
  list<DrawSegsStrategy *>::iterator it = m_oDrawers.begin();
  for( ; it != m_oDrawers.end(); it++ )
  {
    (*it)->showSegs();
  }
}
/*---------------------------------------------------------------------------------*/
void ConcreteDrawableSegs::redrawSegs(void)
{
  list<DrawSegsStrategy *>::iterator it = m_oDrawers.begin();
  for( ; it != m_oDrawers.end(); it++ )
  {
    (*it)->redrawSegs();
  }
}
/*---------------------------------------------------------------------------------*/
}
