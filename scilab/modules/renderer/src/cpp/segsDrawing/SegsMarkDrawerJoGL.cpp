/*------------------------------------------------------------------------*/
/* file: SegsMarkDrawerJoGL.cpp                                           */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Startegy drawing the marks at the end of segment                */
/*------------------------------------------------------------------------*/

#include "SegsMarkDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
SegsMarkDrawerJoGL::SegsMarkDrawerJoGL( DrawableSegs * segs )
  : DrawSegsStrategy(segs), DrawableObjectJoGL(segs)
{
  setJavaMapper(new SegsMarkDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
SegsMarkDrawerJoGL::~SegsMarkDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void SegsMarkDrawerJoGL::drawSegs(const double xStarts[], const double xEnds[],
                                  const double yStarts[], const double yEnds[],
                                  const double zStarts[], const double zEnds[],
                                  const int colors[], int nbSegment)
{
  sciPointObj * pSegs = m_pDrawed->getDrawedObject();

  initializeDrawing();
  getMarkDrawerJavaMapper()->setMarkParameters(sciGetGraphicContext(pSegs)->markbackground,
                                               sciGetGraphicContext(pSegs)->markforeground,
                                               sciGetMarkSizeUnit(pSegs),
                                               sciGetMarkSize(pSegs),
                                               sciGetMarkStyle(pSegs));

  getMarkDrawerJavaMapper()->drawSegs(xStarts, xEnds,
                                      yStarts, yEnds,
                                      zStarts, zEnds,
                                      nbSegment);
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void SegsMarkDrawerJoGL::showSegs( void )
{
  show();
}
/*---------------------------------------------------------------------------------*/
SegsMarkDrawerJavaMapper * SegsMarkDrawerJoGL::getMarkDrawerJavaMapper(void)
{
  return dynamic_cast<SegsMarkDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
