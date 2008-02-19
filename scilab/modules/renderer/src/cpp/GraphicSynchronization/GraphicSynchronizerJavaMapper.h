/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Implementation of GraphicSynchronizer bridge using java methods
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _GRAPHIC_SYNCHRONIZER_JAVA_H_
#define _GRAPHIC_SYNCHRONIZER_JAVA_H_

#include "GraphicSynchronizerBridge.h"
#include "GraphicSynchronizerJava.hxx"

namespace sciGraphics
{

class GraphicSynchronizerJavaMapper : public virtual GraphicSynchronizerBridge
{
public:

  GraphicSynchronizerJavaMapper( void ) ;

  virtual ~GraphicSynchronizerJavaMapper( void ) ;

  /**
   * Protect some part of code to be accessed by several threads.
   */
  virtual void enterCriticalSection( void ) ;

  /**
   * End the a part a code which should not be accessed by several threads
   */
  virtual void exitCriticalSection( void ) ;

  /**
   * Cause the thread to wait until a call to notify or notify all wake him
   */
  virtual void wait( void ) ;

  /**
   * Wake one thread which called the wait method of this object.
   */
  virtual void notify( void ) ;

  /**
   * Wake all the thread which called the wait method of this object.
   */
  virtual void notifyAll( void ) ;

  /**
   * Get the id of the thread currently running.
   */
  virtual int getCurrentThreadId(void);

protected:

  org_scilab_modules_renderer_utils_graphicSynchronization::GraphicSynchronizerJava * m_pJavaObject;

};

}

#endif /* _GRAPHIC_SYNCHRONIZER_JAVA_H_ */
