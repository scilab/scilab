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

extern "C"
{
#include "sciprint.h"
#include "localization.h"
}

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
	// to be sure that the inner structure is up to date.
	update();

  m_pDecomposer->getBoundingBox(bounds);
}
/*---------------------------------------------------------------------------------*/
DrawableObject::EDisplayStatus ConcreteDrawableSegs::drawSegs(void)
{
  int nbSegs = getNbSegment();
  double * xStarts = NULL;
  double * xEnds   = NULL;
  double * yStarts = NULL;
  double * yEnds   = NULL;
  double * zStarts = NULL;
  double * zEnds   = NULL;
  int * colors = NULL;

  try
  {
    // allocation
    xStarts = new double[nbSegs];
    xEnds   = new double[nbSegs];
    yStarts = new double[nbSegs];
    yEnds   = new double[nbSegs];
    zStarts = new double[nbSegs];
    zEnds   = new double[nbSegs];
    if (isColored())
    {
      colors  = new int[nbSegs];
    }
  }
  catch (const std::exception& e)
  {
    // allocation failed
    sciprint(const_cast<char*>(_("%s: No more memory.\n")),"ConcreteDrawableSegs::drawSegs");
    if(xStarts != NULL) { delete[] xStarts; }
    if(xEnds != NULL)   { delete[] xEnds; }
    if(yStarts != NULL) { delete[] yStarts; }
    if(yEnds != NULL)   { delete[] yEnds; }
    if(zStarts != NULL) { delete[] zStarts; }
    if(zEnds != NULL)   { delete[] zEnds; }
    if(colors != NULL)  { delete[] colors; }
    return FAILURE;
  }

  /* Compute position of each arrow */
  getSegsPos(xStarts, xEnds, yStarts, yEnds, zStarts, zEnds);

  if (isColored())
  {
    /* get color of each arrow */
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
  
  return SUCCESS;

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
