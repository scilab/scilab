/*------------------------------------------------------------------------*/
/* file: CameraBridgeFactory.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for CameraBridge class                                  */
/*------------------------------------------------------------------------*/

#include "CameraBridgeFactory.h"
#include "CameraJoGL.h"
#include "CameraJavaMapper.hxx"

namespace sciGraphics
{

/*------------------------------------------------------------------------*/
CameraBridge * CameraBridgeFactory::create( void )
{
  CameraJoGL * newBridge = new CameraJoGL(m_pSubwin) ;
  newBridge->setJavaMapper(new CameraJavaMapper()) ;
  return newBridge;
}
/*------------------------------------------------------------------------*/

}
