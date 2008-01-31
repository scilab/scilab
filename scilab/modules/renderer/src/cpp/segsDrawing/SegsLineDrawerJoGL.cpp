/*------------------------------------------------------------------------*/
/* file: SegsLineDrawerJoGL.cpp                                           */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Startegy drawing the line part of segment arrows                */
/*------------------------------------------------------------------------*/

#include "SegsLineDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
SegsLineDrawerJoGL::SegsLineDrawerJoGL( DrawableSegs * segs )
: DrawSegsStrategy(segs), DrawableObjectJoGL(segs)
{
  setJavaMapper(new SegsLineDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
SegsLineDrawerJoGL::~SegsLineDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJoGL::drawSegs(const double xStarts[], const double xEnds[],
                                  const double yStarts[], const double yEnds[],
                                  const double zStarts[], const double zEnds[],
                                  const int colors[], int nbSegment)
{
  sciPointObj * pSegs = m_pDrawed->getDrawedObject();

  initializeDrawing();
  getLineDrawerJavaMapper()->setLineParameters((float) sciGetLineWidth(pSegs), sciGetLineStyle(pSegs));
  getLineDrawerJavaMapper()->drawSegs(xStarts, xEnds,
                                      yStarts, yEnds,
                                      zStarts, zEnds,
                                      colors, nbSegment);
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void SegsLineDrawerJoGL::showSegs( void )
{
  show();
}
/*---------------------------------------------------------------------------------*/
SegsLineDrawerJavaMapper * SegsLineDrawerJoGL::getLineDrawerJavaMapper(void)
{
  return dynamic_cast<SegsLineDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
