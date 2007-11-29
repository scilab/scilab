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
  double * bottom     = NULL;
  double * top        = NULL;
  double * left       = NULL;
  double * right      = NULL;

  // special case here, we must use a special decomposition for bars.
  BarDecomposition decomposer(m_pDrawed);

  nbVertices = decomposer.getDrawnVerticesLength();
  xCoords = new double[nbVertices];
  yCoords = new double[nbVertices];
  zCoords = new double[nbVertices];
  bottom  = new double[nbVertices];
  top     = new double[nbVertices];
  left    = new double[nbVertices];
  right   = new double[nbVertices]; 

  if (   xCoords == NULL || yCoords == NULL || zCoords == NULL
      || bottom == NULL || top == NULL || left == NULL || right == NULL)
  {
    sciprint(_("Unable to render polyline, memory full.\n"));
    if (xCoords != NULL) { delete[] xCoords; }
    if (yCoords != NULL) { delete[] yCoords; }
    if (zCoords != NULL) { delete[] zCoords; }
    if (bottom  != NULL) { delete[] bottom;  }
    if (top     != NULL) { delete[] top;     }
    if (left    != NULL) { delete[] left;    }
    if (right   != NULL) { delete[] right;   }
    endDrawing();
    return;
  }

  decomposer.getDrawnVertices(xCoords, yCoords, zCoords);
  decomposer.getBarOrdinates(bottom, top);
  decomposer.getBarAbscissas(left, right);

  // display the rectangle
  getBarDrawerJavaMapper()->drawPolyline(left, right, bottom, top, zCoords, nbVertices);
 
  delete[] xCoords;
  delete[] yCoords;
  delete[] zCoords;
  delete[] bottom;
  delete[] top;
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
