/*------------------------------------------------------------------------*/
/* file: Camera.cpp                                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : File used to position viewpoint and rendering zone in the       */
/*        canvas.                                                         */
/*------------------------------------------------------------------------*/

#include "Camera.h"
#include "DrawableSubwin.h"

extern "C"
{
#include "math_graphics.h"
#include "GetProperty.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
Camera::Camera( sciPointObj * pObj ) : DrawableObject(pObj)
{
  
}
/*--------------------------------------------------------------------------*/
Camera::~Camera( void )
{
  
}
/*--------------------------------------------------------------------------*/
void Camera::setViewingArea( double axesBounds[4], double margins[4] )
{
  double translation[2] ;
  // T = Taxes + SizeAxes * [marginLeft, marginTop].
  translation[0] = axesBounds[0] + axesBounds[2] * margins[0] ;
  translation[1] = axesBounds[1] + axesBounds[3] * margins[2] ;

  // S = Saxes - margin.Saxes
  // where Saxes = [w,h] and margin = [margin_left+margin_right, margin_top+margin_bottom].
  double scale[2] ;
  scale[0] = (1.0 - margins[0] - margins[1]) * axesBounds[2] ;
  scale[1] = (1.0 - margins[2] - margins[3]) * axesBounds[3] ;
  getCameraImp()->setViewingArea(translation, scale);
}
/*--------------------------------------------------------------------------*/
void Camera::setRotationAngles( double alpha, double theta )
{
  getCameraImp()->setAxesRotation(alpha, theta);
}
/*--------------------------------------------------------------------------*/
void Camera::setAxesReverse(BOOL axesReverse[3])
{
  getCameraImp()->setAxesReverse(axesReverse[0] == TRUE,
                                 axesReverse[1] == TRUE,
                                 axesReverse[2] == TRUE);
}
/*--------------------------------------------------------------------------*/
void Camera::setSubwinBox( double bounds[6] )
{

  double boxCenter[3];
  boxCenter[0] = (bounds[0] + bounds[1]) / 2.0 ;
  boxCenter[1] = (bounds[2] + bounds[3]) / 2.0 ;
  boxCenter[2] = (bounds[4] + bounds[5]) / 2.0 ;
  getCameraImp()->setAxesCenter(boxCenter) ;
  
  double scale[3] ;
  // 1.0 / ( Xmax - Xmin )
  scale[0] = 1.0 / (bounds[1] - bounds[0]) ;
  scale[1] = 1.0 / (bounds[3] - bounds[2]) ;
  scale[2] = 1.0 / (bounds[5] - bounds[4]) ;
  
  getCameraImp()->setAxesNormalizationScale(scale) ;

  if (sciGetIsCubeScaled(m_pDrawed))
  {
    getCameraImp()->setAxesFittingScale(scale);
  }
  else
  {
    // preserve isometry by applying same scale
    double minScale = Min(scale[0], Min(scale[1], scale[2]));
    double fittingScale[3] = {minScale, minScale, minScale};
    getCameraImp()->setAxesFittingScale(fittingScale);
  }
  

  double trans[3];
  // put the min bounds to our origin
  trans[0] = -bounds[0] ;
  trans[1] = -bounds[2] ;
  trans[2] = bounds[4] ;
  getCameraImp()->setAxesTranslation(trans) ;

}
/*--------------------------------------------------------------------------*/
void Camera::setCameraParameters(void)
{
  // here m_pDrawed is the subwin
  setViewingArea(sciGetAxesBounds(m_pDrawed), sciGetMargins(m_pDrawed)) ;

  double bounds[6] ;
  sciGetRealDataBounds(m_pDrawed, bounds) ;
  setSubwinBox(bounds) ;

  double alpha;
  double theta;
  sciGetViewingAngles(m_pDrawed, &alpha, &theta);
  setRotationAngles(alpha, theta);

  BOOL axesReverse[3];
  sciGetAxesReverse(m_pDrawed, axesReverse);
  setAxesReverse(axesReverse);
}
/*--------------------------------------------------------------------------*/
void Camera::renderPosition( void )
{
  getCameraImp()->renderPosition();
}
/*--------------------------------------------------------------------------*/
void Camera::replaceCamera( void )
{
  getCameraImp()->replaceCamera();
}
/*--------------------------------------------------------------------------*/
void Camera::getPixelCoordinates(const double userCoord[3], int pixCoord[2])
{
  getCameraImp()->getPixelCoordinates(userCoord, pixCoord);
}
/*--------------------------------------------------------------------------*/
void Camera::get2dViewPixelCoordinates(const double userCoord[3], int pixCoord[2])
{
  getCameraImp()->get2dViewPixelCoordinates(userCoord, pixCoord);
}
/*--------------------------------------------------------------------------*/
void Camera::draw( void )
{
  initializeDrawing();
  setCameraParameters();
  getCameraImp()->renderPosition();
  endDrawing();
}
/*--------------------------------------------------------------------------*/
void Camera::show( void )
{
  initializeDrawing();
  getCameraImp()->show();
  endDrawing();
}
/*--------------------------------------------------------------------------*/
CameraBridge * Camera::getCameraImp( void )
{
  return dynamic_cast<CameraBridge *>(m_pImp);
}
/*--------------------------------------------------------------------------*/
}

#undef FIT_WINDOW_RATIO
