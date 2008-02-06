/*------------------------------------------------------------------------*/
/* file: FecLineDrawerJoGL.cpp                                            */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy for the mesh of a fec object                           */
/*------------------------------------------------------------------------*/

#include "FecLineDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
FecLineDrawerJoGL::FecLineDrawerJoGL(DrawableFec * fec)
: DrawFecStrategy(), DrawableObjectJoGL(fec)
{
  setJavaMapper(new FecLineDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
FecLineDrawerJoGL::~FecLineDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void FecLineDrawerJoGL::drawFec(const double xCoords[], const double yCoords[],
                                const double values[], int nbNodes,
                                const int firstPoints[], const int secondPoints[],
                                const int thirdPoints[], int nbTriangles)
{
  sciPointObj * pFec = getDrawer()->getDrawedObject();
  initializeDrawing();
  getLineDrawerJavaMapper()->setLineParameters(sciGetGraphicContext(pFec)->foregroundcolor,
                                               (float)sciGetLineWidth(pFec),
                                               sciGetLineStyle(pFec));

  getLineDrawerJavaMapper()->drawFec(xCoords, yCoords, values, nbNodes,
                                     firstPoints, secondPoints, thirdPoints, nbTriangles);
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
void FecLineDrawerJoGL::showFec(void)
{
  show();
}
/*---------------------------------------------------------------------------------*/
DrawableFec * FecLineDrawerJoGL::getFecDrawer(void)
{
  return dynamic_cast<DrawableFec *>(getDrawer());
}
/*---------------------------------------------------------------------------------*/
FecLineDrawerJavaMapper * FecLineDrawerJoGL::getLineDrawerJavaMapper(void)
{
  return dynamic_cast<FecLineDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
