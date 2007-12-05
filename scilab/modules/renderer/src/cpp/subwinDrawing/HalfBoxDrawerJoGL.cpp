/*------------------------------------------------------------------------*/
/* file: HalfBoxDrawerJoGL.cpp                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class drawing half of a subwin object bounding box              */
/*------------------------------------------------------------------------*/

#include "HalfBoxDrawerJoGL.hxx"
#include "HalfBoxDrawerJavaMapper.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
HalfBoxDrawerJoGL::HalfBoxDrawerJoGL(DrawableSubwin * subwin)
  : DrawAxesBoxStrategy(), DrawableObjectJoGL(subwin)
{
  setJavaMapper(new HalfBoxDrawerJavaMapper());
}
/*--------------------------------------------------------------------------*/
HalfBoxDrawerJoGL::~HalfBoxDrawerJoGL(void)
{

}
/*--------------------------------------------------------------------------*/
void HalfBoxDrawerJoGL::drawAxesBox(void)
{
  sciPointObj * pSubwin = m_pDrawer->getDrawedObject();
  initializeDrawing();

  getHalfBoxDrawerJavaMapper()->setBoxParameters(pSUBWIN_FEATURE(pSubwin)->hiddencolor,
                                                 sciGetGraphicContext(pSubwin)->backgroundcolor,
                                                 sciGetGraphicContext(pSubwin)->foregroundcolor,
                                                 sciGetLineStyle(pSubwin),
                                                 (float) sciGetLineWidth(pSubwin));

  // get displayed bounds
  double bounds[6];
  sciGetRealDataBounds(pSubwin, bounds);
  getHalfBoxDrawerJavaMapper()->drawBox(bounds[0], bounds[1], bounds[2],
                                    bounds[3], bounds[4], bounds[5]);
  endDrawing();
}
/*--------------------------------------------------------------------------*/
HalfBoxDrawerJavaMapper * HalfBoxDrawerJoGL::getHalfBoxDrawerJavaMapper(void)
{
  return dynamic_cast<HalfBoxDrawerJavaMapper *>(getJavaMapper());
}
/*--------------------------------------------------------------------------*/
}
