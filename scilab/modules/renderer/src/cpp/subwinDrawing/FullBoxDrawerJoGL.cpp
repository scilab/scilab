/*------------------------------------------------------------------------*/
/* file: FullBoxDrawerJoGL.cpp                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing the full bounding box of a subwin object          */
/*------------------------------------------------------------------------*/

#include "FullBoxDrawerJoGL.hxx"
#include "FullBoxDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
FullBoxDrawerJoGL::FullBoxDrawerJoGL(DrawableSubwin * subwin)
  : DrawAxesBoxStrategy(), DrawableObjectJoGL(subwin)
{
  setJavaMapper(new FullBoxDrawerJavaMapper());
}
/*--------------------------------------------------------------------------*/
FullBoxDrawerJoGL::~FullBoxDrawerJoGL(void)
{

}
/*--------------------------------------------------------------------------*/
void FullBoxDrawerJoGL::drawAxesBox(void)
{
  sciPointObj * pSubwin = m_pDrawer->getDrawedObject();
  initializeDrawing();

  getFullBoxDrawerJavaMapper()->setBoxParameters(pSUBWIN_FEATURE(pSubwin)->hiddencolor,
                                                 sciGetGraphicContext(pSubwin)->backgroundcolor,
                                                 sciGetGraphicContext(pSubwin)->foregroundcolor,
                                                 sciGetLineStyle(pSubwin),
                                                 (float) sciGetLineWidth(pSubwin));

  // get displayed bounds
  double bounds[6];
  sciGetRealDataBounds(pSubwin, bounds);
  getFullBoxDrawerJavaMapper()->drawBox(bounds[0], bounds[1], bounds[2],
                                        bounds[3], bounds[4], bounds[5]);
  endDrawing();
}
/*--------------------------------------------------------------------------*/
FullBoxDrawerJavaMapper * FullBoxDrawerJoGL::getFullBoxDrawerJavaMapper(void)
{
  return dynamic_cast<FullBoxDrawerJavaMapper *>(getJavaMapper());
}
/*--------------------------------------------------------------------------*/
}
