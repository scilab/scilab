/*------------------------------------------------------------------------*/
/* file: ConcreteDrawableSegs.cpp                                         */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing implementation of driver independent           */
/*        algorithms of drawable segs                                     */
/*------------------------------------------------------------------------*/

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
}
