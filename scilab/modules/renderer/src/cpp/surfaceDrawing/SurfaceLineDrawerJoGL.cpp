/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Strategy drawing lines surrounding a surface object facets
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "SurfaceLineDrawerJoGL.hxx"

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
SurfaceLineDrawerJoGL::SurfaceLineDrawerJoGL( DrawableSurface * surface )
  : DrawSurfaceStrategy(surface), DrawableObjectJoGL(surface)
{
  setJavaMapper(new SurfaceLineDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
SurfaceLineDrawerJoGL::~SurfaceLineDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void SurfaceLineDrawerJoGL::drawSurface( void )
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
    sciprint(_("%s: No more memory.\n"),"SurfaceLineDrawerJoGL::drawSurface");
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
  getLineDrawerJavaMapper()->setSurfaceType(ppSurface->typeof3d);
  getLineDrawerJavaMapper()->setLineParameters(sciGetGraphicContext(pSurface)->foregroundcolor,
                                               (float) sciGetLineWidth(pSurface),
                                               sciGetLineStyle(pSurface));
  try
  {
    getLineDrawerJavaMapper()->drawSurface(xCoords, sizeXCoord,
                                           yCoords, sizeYCoord,
                                           zCoords, sizeZCoord,
                                           nbVertexPerFacet);
  }
  catch (std::exception & e)
  {
    sciprint(_("%s: No more memory.\n"),"SurfaceLineDrawerJoGL ::drawSurface");
  }

  endDrawing();

  delete[] xCoords;
  delete[] yCoords;
  delete[] zCoords;
}
/*---------------------------------------------------------------------------------*/
void SurfaceLineDrawerJoGL::showSurface( void )
{
  show();
}
/*---------------------------------------------------------------------------------*/
SurfaceLineDrawerJavaMapper * SurfaceLineDrawerJoGL::getLineDrawerJavaMapper(void)
{
  return dynamic_cast<SurfaceLineDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/
}
