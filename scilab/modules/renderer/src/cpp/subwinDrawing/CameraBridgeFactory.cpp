/*------------------------------------------------------------------------*/
/* file: CameraBridgeFactory.cpp                                          */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Factory for CameraBridge class                                  */
/*------------------------------------------------------------------------*/

#include "CameraBridgeFactory.h"
#include "CameraJoGL.h"

namespace sciGraphics
{

/*------------------------------------------------------------------------*/
CameraBridge * CameraBridgeFactory::create( void )
{
  return new CameraJoGL(m_pSubwin) ;
}
/*------------------------------------------------------------------------*/

}
