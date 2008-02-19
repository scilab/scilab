/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Driver dependent routines of GraphicSynchronizer class
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _GRAPHIC_SYNCHRONIZER_BRIDGE_H_
#define _GRAPHIC_SYNCHRONIZER_BRIDGE_H_

namespace sciGraphics
{

class GraphicSynchronizerBridge
{
public:

  GraphicSynchronizerBridge( void ) {}

  virtual ~GraphicSynchronizerBridge( void ) {}

  /**
   * Protect some part of code to be accessed by several threads.
   */
  virtual void enterCriticalSection( void ) = 0 ;

  /**
   * End the a part a code which should not be accessed by several threads
   */
  virtual void exitCriticalSection( void ) = 0 ;

  /**
   * Cause the thread to wait until a call to notify or notify all wake him
   */
  virtual void wait( void ) = 0 ;

  /**
   * Wake one thread which called the wait method of this object.
   */
  virtual void notify( void ) = 0 ;

  /**
   * Wake all the thread which called the wait method of this object.
   */
  virtual void notifyAll( void ) = 0 ;

  /**
   * Get the id of the thread currently running.
   */
  virtual int getCurrentThreadId(void) = 0;

};

}

#endif /* _GRAPHIC_SYNCHRONIZER_BRIDGE_H_ */
