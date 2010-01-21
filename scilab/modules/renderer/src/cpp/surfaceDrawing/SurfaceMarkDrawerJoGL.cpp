/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy drawing marks on the vertices of surface object facets
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SurfaceMarkDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
#include "math_graphics.h"
#include "BasicAlgos.h"
#include "sciprint.h"
#include "localization.h"
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
  double * xCoords = NULL; 
  double * yCoords = NULL;
  double * zCoords = NULL;

  try
  {
    xCoords = new double[sizeXCoord];
    yCoords = new double[sizeYCoord];
    zCoords = new double[sizeZCoord];
  }
  catch (const std::exception& e)
  {
    // allocation failed
    sciprint(const_cast<char*>(_("%s: No more memory.\n")),"SurfaceMarkDrawerJoGL::drawSurface");
    if(xCoords != NULL) { delete[] xCoords; }
    if(yCoords != NULL) { delete[] yCoords; }
    if(zCoords != NULL) { delete[] zCoords; }
    endDrawing();
    return;
  }


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
  try
  {
    getMarkDrawerJavaMapper()->drawSurface(xCoords, sizeXCoord,
                                           yCoords, sizeYCoord,
                                           zCoords, sizeZCoord,
                                           nbVertexPerFacet);
  }
  catch (std::exception & e)
  {
    sciprint(const_cast<char*>(_("%s: No more memory.\n")),"SurfaceMarkDrawerJoGL::drawSurface");
  }
  

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
void SurfaceMarkDrawerJoGL::redrawSurface( void )
{
  initializeDrawing();
  getMarkDrawerJavaMapper()->drawSurface();
  endDrawing();
}
/*---------------------------------------------------------------------------------*/
SurfaceMarkDrawerJavaMapper * SurfaceMarkDrawerJoGL::getMarkDrawerJavaMapper(void)
{
  return dynamic_cast<SurfaceMarkDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
