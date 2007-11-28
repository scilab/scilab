/*------------------------------------------------------------------------*/
/* file: PolylineBarDrawerJoGL.cpp                                        */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Strategy drawing bar plots                                      */
/*------------------------------------------------------------------------*/


#include "PolylineBarDrawerJoGL.hxx"
#include "BarDecomposition.hxx"

extern "C"
{
#include "GetProperty.h"
#include "sciprint.h"
#include "localization.h"
};

namespace sciGraphics
{

/*---------------------------------------------------------------------------------*/
PolylineBarDrawerJoGL::PolylineBarDrawerJoGL( DrawablePolyline * polyline )
  : DrawPolylineStrategy(polyline), DrawableObjectJoGL(polyline)
{
  setJavaMapper(new PolylineBarDrawerJavaMapper());
}
/*---------------------------------------------------------------------------------*/
PolylineBarDrawerJoGL::~PolylineBarDrawerJoGL(void)
{

}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJoGL::drawPolyline( void )
{
  sciPointObj * pObj = m_pDrawed->getDrawedObject();
  initializeDrawing() ;


  

  // set the line parameters
  getBarDrawerJavaMapper()->setBarParameters(sciGetGraphicContext(pObj)->backgroundcolor,
                                             sciGetGraphicContext(pObj)->foregroundcolor,
                                            (float)sciGetLineWidth(pObj),
                                             sciGetLineStyle(pObj));

  // get the data of the polyline
  int      nbVertices = 0   ;
  double * xCoords    = NULL;
  double * yCoords    = NULL;
  double * zCoords    = NULL;
  double * height     = NULL;
  double * left       = NULL;
  double * right      = NULL;

  // special case here, we must use a special decomposition for bars.
  BarDecomposition decomposer(m_pDrawed);

  nbVertices = decomposer.getDrawnVerticesLength();
  xCoords = new double[nbVertices];
  yCoords = new double[nbVertices];
  zCoords = new double[nbVertices];
  height  = new double[nbVertices];
  left    = new double[nbVertices];
  right   = new double[nbVertices]; 

  if (   xCoords == NULL || yCoords == NULL || zCoords == NULL
      || height == NULL || left == NULL || right == NULL)
  {
    sciprint(_("Unable to render polyline, memory full.\n"));
    if (xCoords != NULL) { delete[] xCoords; }
    if (yCoords != NULL) { delete[] yCoords; }
    if (zCoords != NULL) { delete[] zCoords; }
    if (height  != NULL) { delete[] height;  }
    if (left    != NULL) { delete[] left;    }
    if (right   != NULL) { delete[] right;   }
    endDrawing();
    return;
  }

  decomposer.getDrawnVertices(xCoords, yCoords, zCoords);
  decomposer.getBarHeight(height);
  decomposer.getBarWidth(left, right);

  // display the rectangle
  getBarDrawerJavaMapper()->drawPolyline(xCoords, yCoords, zCoords, height, left, right, nbVertices);
 
  delete[] xCoords;
  delete[] yCoords;
  delete[] zCoords;
  delete[] height;
  delete[] left;
  delete[] right;
  endDrawing() ;
}
/*---------------------------------------------------------------------------------*/
void PolylineBarDrawerJoGL::showPolyline( void )
{
  show();
}
/*---------------------------------------------------------------------------------*/
PolylineBarDrawerJavaMapper * PolylineBarDrawerJoGL::getBarDrawerJavaMapper(void)
{
  return dynamic_cast<PolylineBarDrawerJavaMapper *>(getJavaMapper());
}
/*---------------------------------------------------------------------------------*/

}
