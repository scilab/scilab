/*------------------------------------------------------------------------*/
/* file: PolylineMarkDrawerJoGL.hxx                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing the marks on a polyline object                 */
/*------------------------------------------------------------------------*/

#include "PolylineMarkDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
};

#include "BarDecomposition.hxx"

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
PolylineMarkDrawerJoGL::PolylineMarkDrawerJoGL( DrawablePolyline * polyline )
  : DrawPolylineStrategy(polyline), DrawableObjectJoGL(polyline)
{
  setJavaMapper(new PolylineMarkDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
PolylineMarkDrawerJoGL::~PolylineMarkDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void PolylineMarkDrawerJoGL::drawPolyline( void )
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();
  initializeDrawing() ;

  // set the line parameters
  getMarkDrawerJavaMapper()->setMarkParameters(sciGetGraphicContext(pObj)->markbackground,
                                               sciGetGraphicContext(pObj)->markforeground,
                                               sciGetMarkSizeUnit(pObj),
                                               sciGetMarkSize(pObj),
                                               sciGetMarkStyle(pObj));

  // get the data of the polyline
  int      nbVertices = 0   ;
  double * xCoords    = NULL;
  double * yCoords    = NULL;
  double * zCoords    = NULL;

  // special case for bar plot
  BarDecomposition barPoltDecomposer(m_pDrawed);

  if (sciGetPolylineStyle(pObj) == 3)
  {
    // bar plot also draw marks on y = 0 plane
    nbVertices = barPoltDecomposer.getBarPlotMarkVerticesLength();
  }
  else
  {
    nbVertices = m_pDrawed->getDrawnVerticesLength();
  }
  
  xCoords = new double[nbVertices];
  yCoords = new double[nbVertices];
  zCoords = new double[nbVertices];

  if (xCoords == NULL || yCoords == NULL || zCoords == NULL)
  {
	  sciprint(_("%s: No more memory.\n"),"PolylineMarkDrawerJoGL::drawPolyline");
	  if(xCoords != NULL) { delete[] xCoords; }
	  if(yCoords != NULL) { delete[] yCoords; }
	  if(zCoords != NULL) { delete[] zCoords; }
	  endDrawing();
	  return;
  }

  if (sciGetPolylineStyle(pObj) == 3)
  {
    barPoltDecomposer.getBarPlotMarkVertices(xCoords, yCoords, zCoords);
  }
  else
  {
    m_pDrawed->getDrawnVertices(xCoords, yCoords, zCoords);
  }

  // display the rectangle
  getMarkDrawerJavaMapper()->drawPolyline(xCoords, yCoords, zCoords, nbVertices);

  delete[] xCoords;
  delete[] yCoords;
  delete[] zCoords;
  endDrawing() ;
}
/*---------------------------------------------------------------------------------*/
void PolylineMarkDrawerJoGL::showPolyline( void )
{
  show();
}
/*---------------------------------------------------------------------------------*/
PolylineMarkDrawerJavaMapper * PolylineMarkDrawerJoGL::getMarkDrawerJavaMapper(void)
{
  return dynamic_cast<PolylineMarkDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/

}
