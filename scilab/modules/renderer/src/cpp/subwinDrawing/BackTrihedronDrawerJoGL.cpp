/*------------------------------------------------------------------------*/
/* file: BackTrihedronDrawer.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing only the backTrihedron of a subwin object         */
/*------------------------------------------------------------------------*/

#include "BackTrihedronDrawerJoGL.hxx"
#include "BackTrihedronDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
BackTrihedronDrawerJoGL::BackTrihedronDrawerJoGL(DrawableSubwin * subwin)
  : DrawAxesBoxStrategy(), DrawableObjectJoGL(subwin)
{
  setJavaMapper(new BackTrihedronDrawerJavaMapper());
}
/*--------------------------------------------------------------------------*/
BackTrihedronDrawerJoGL::~BackTrihedronDrawerJoGL(void)
{
  
}
/*--------------------------------------------------------------------------*/
void BackTrihedronDrawerJoGL::drawAxesBox(void)
{
  sciPointObj * pSubwin = m_pDrawer->getDrawedObject();
  initializeDrawing();

  getBackTrihedronDrawerJavaMapper()->setBoxParameters(pSUBWIN_FEATURE(pSubwin)->hiddencolor,
                                                       sciGetGraphicContext(pSubwin)->backgroundcolor,
                                                       sciGetLineStyle(pSubwin),
                                                       (float) sciGetLineWidth(pSubwin));

  // get displayed bounds
  double bounds[6];
  sciGetRealDataBounds(pSubwin, bounds);
  getBackTrihedronDrawerJavaMapper()->drawBox(bounds[0], bounds[1], bounds[2],
                                              bounds[3], bounds[4], bounds[5]);
  endDrawing();
}
/*--------------------------------------------------------------------------*/
BackTrihedronDrawerJavaMapper * BackTrihedronDrawerJoGL::getBackTrihedronDrawerJavaMapper(void)
{
  return dynamic_cast<BackTrihedronDrawerJavaMapper *>(getJavaMapper());
}
/*--------------------------------------------------------------------------*/
}
