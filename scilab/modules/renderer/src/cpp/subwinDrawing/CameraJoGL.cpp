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
CameraJoGL::CameraJoGL( DrawableObject * subwin )
  : DrawableObjectImp(subwin), CameraBridge(),
    DrawableObjectJoGL(subwin, "org/scilab/modules/renderer/subwinDrawing/CameraJoGL")
{

}
/*-----------------------------------------------------------------------------------*/
void CameraJoGL::renderPosition( void )
{
  initializeDrawing();
  jniCallVoidFunctionSafe( m_oDrawableObject, m_oDrawableClass, "moveViewingArea", "");
  endDrawing();
}
/*-----------------------------------------------------------------------------------*/

}
