/*------------------------------------------------------------------------*/
/* file: SurfaceMarkDrawerJoGL.cpp                                        */
/* Copyright INRIA 2008                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing marks on the vertices of surface object        */
/*        facets                                                          */
/*------------------------------------------------------------------------*/

#include "SurfaceMarkDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
#include "math_graphics.h"
#include "BasicAlgos.h"
}

namespace sciGraphics
{
/*---------------------------------------------------------------------------------*/
SurfaceMarkDrawerJoGL::SurfaceMarkDrawerJoGL( DrawableSurface * surface )
  : DrawSurfaceStrategy(surface), DrawableObjectJoGL(surface)
{
  setJavaMapper(new SurfaceMarkDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
SurfaceMarkDrawerJoGL::~SurfaceMarkDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void SurfaceMarkDrawerJoGL::drawSurface( void )
{
  sciPointObj * pSurface = m_pDrawer->getDrawedObject();
  sciSurface * ppSurface = pSURFACE_FEATURE(pSurface);

  int nbVertexPerFacet = 0;
  if (ppSurface->typeof3d == SCI_PLOT3D)
  {
    nbVertexPerFacet = 4;
  }
  else
  {
    // that what's was in the old code
    nbVertexPerFacet = ppSurface->dimzx;
  }

  int sizeXCoord = ppSurface->nx;
  int sizeYCoord = ppSurface->ny;
  int sizeZCoord = ppSurface->nz;
  double * xCoords = new double[sizeXCoord];
  double * yCoords = new double[sizeYCoord];
  double * zCoords = new double[sizeZCoord];
  doubleArrayCopy(xCoords, ppSurface->pvecx, sizeXCoord);
  doubleArrayCopy(yCoords, ppSurface->pvecy, sizeYCoord);
  doubleArrayCopy(zCoords, ppSurface->pvecz, sizeZCoord);

  m_pDrawed->pointScale(xCoords, NULL, NULL, sizeXCoord);
  m_pDrawed->pointScale(NULL, yCoords, NULL, sizeYCoord);
  m_pDrawed->pointScale(NULL, NULL, zCoords, sizeZCoord);

  initializeDrawing();
  getMarkDrawerJavaMapper()->setSurfaceType(ppSurface->typeof3d);
  getMarkDrawerJavaMapper()->setMarkParameters(sciGetGraphicContext(pSurface)->markbackground,
                                               sciGetGraphicContext(pSurface)->markforeground,
                                               sciGetMarkSizeUnit(pSurface),
                                               sciGetMarkSize(pSurface),
                                               sciGetMarkStyle(pSurface));
  getMarkDrawerJavaMapper()->drawSurface(xCoords, sizeXCoord,
                                         yCoords, sizeYCoord,
                                         zCoords, sizeZCoord,
                                         nbVertexPerFacet);
  

  endDrawing();

  delete[] xCoords;
  delete[] yCoords;
  delete[] zCoords;

}
/*---------------------------------------------------------------------------------*/
void SurfaceMarkDrawerJoGL::showSurface( void )
{
  show();
}
/*---------------------------------------------------------------------------------*/
SurfaceMarkDrawerJavaMapper * SurfaceMarkDrawerJoGL::getMarkDrawerJavaMapper(void)
{
  return dynamic_cast<SurfaceMarkDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
