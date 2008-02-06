/*------------------------------------------------------------------------*/
/* file: FecFacetDrawerJoGL.cpp                                           */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for the facets of a fec object                         */
/*------------------------------------------------------------------------*/

#include "FecFacetDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
FecFacetDrawerJoGL::FecFacetDrawerJoGL(DrawableFec * fec)
  : DrawFecStrategy(), DrawableObjectJoGL(fec)
{
  setJavaMapper(new FecFacetDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
FecFacetDrawerJoGL::~FecFacetDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJoGL::drawFec(const double xCoords[], const double yCoords[],
                                 const double values[], int nbNodes,
  const int firstPoints[], const int secondPoints[],
  const int thirdPoints[], int nbTriangles)
{
  sciPointObj * pFec = getDrawer()->getDrawedObject();
  sciFec * ppFec = pFEC_FEATURE(pFec);
  initializeDrawing();

  if (ppFec->colminmax[0] == 0 && ppFec->colminmax[1] == 0)
  {
    ppFec->colminmax[0] = 1;
    ppFec->colminmax[1] = sciGetNumColors(sciGetParentFigure(pFec));
  }

  getFacetDrawerJavaMapper()->setFacetParameters(ppFec->zminmax[0], ppFec->zminmax[1],
                                                 ppFec->colminmax[0], ppFec->colminmax[1],
                                                 ppFec->colout[0], ppFec->colout[1]);

  getFacetDrawerJavaMapper()->drawFec(xCoords, yCoords, values, nbNodes,
                                      firstPoints, secondPoints, thirdPoints, nbTriangles);
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void FecFacetDrawerJoGL::showFec(void)
{
  show();
}
/*---------------------------------------------------------------------------------*/
DrawableFec * FecFacetDrawerJoGL::getFecDrawer(void)
{
  return dynamic_cast<DrawableFec *>(getDrawer());
}
/*---------------------------------------------------------------------------------*/
FecFacetDrawerJavaMapper * FecFacetDrawerJoGL::getFacetDrawerJavaMapper(void)
{
  return dynamic_cast<FecFacetDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
