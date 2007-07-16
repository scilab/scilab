/*------------------------------------------------------------------------*/
/* file: CameraJoGL.cpp                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : JoGL class to draw a camera                                     */
/*------------------------------------------------------------------------*/

#include "CameraJoGL.h"
#include "DrawableSubwin.h"
extern "C"
{
#include "JniUtils.h"
}


namespace sciGraphics
{

/*-----------------------------------------------------------------------------------*/
CameraJoGL::CameraJoGL( DrawableSubwin * subwin )
  : CameraBridge(),
    DrawableObjectJoGL(subwin, "org/scilab/modules/renderer/subwinDrawing/CameraJoGL")
{

}
/*-----------------------------------------------------------------------------------*/
void CameraJoGL::renderPosition( void )
{
  initializeDrawing();
  jniCallMemberFunctionSafe( m_oDrawableObject, NULL, "moveViewingArea", "(DDDD)V",
                             m_aViewingTranslation[0], m_aViewingTranslation[1],
                             m_aViewingScale[0], m_aViewingScale[1] ) ;
  jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "moveAxesBox", "(DDDDDD)V",
                            m_aAxesScale[0], m_aAxesScale[1], m_aAxesScale[2],
                            m_aAxesTranslation[0], m_aAxesTranslation[1], m_aAxesTranslation[2] ) ;
  jniCallMemberFunctionSafe(m_oDrawableObject, NULL, "rotateAxesBox", "(DDDDDD)V",
                            m_aBoxCenter[0], m_aBoxCenter[1], m_aBoxCenter[2],
                            m_dAlpha, m_dTheta, m_dFitWindowRatio ) ;
  endDrawing();
}
/*-----------------------------------------------------------------------------------*/

}
