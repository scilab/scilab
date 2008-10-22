/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Synchronizer able to protect the whole graphic data
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _GLOBAL_SYNCHRONIZER_H_
#define _GLOBAL_SYNCHRONIZER_H_

#include "GraphicSynchronizer.h"
#include "GraphicSynchronizerBridge.h"

namespace sciGraphics
{

/**
 * Synchronizer able to protect the whole graphic data 
 */
class GlobalSynchronizer : public GraphicSynchronizer
{

  friend class LocalSynchronizer;
  friend class GraphicSynchronizerFactory;

public:

  virtual ~GlobalSynchronizer( void ) ;
  
  /**
   * To know if local synchronizer can write data
   */
  bool areDataWritable( int threadId ) ;

  /**
   * To know if local synchronizer can write data
   */
  bool areDataReadable( int threadId ) ;

  /**
   * To know if local synchronizer can write data
   */
  bool areDataDisplayable( int threadId ) ;

  /**
   * Set the implementation for driver dependent routines
   */
  void setGraphicSynchronizerImp( GraphicSynchronizerBridge * bridge ) { m_pBridge = bridge ; }

protected:

  /**
   * Default constructor, factory should be called instead.
   */
  GlobalSynchronizer( void ) ;

  /*-----------------------------------------------------------------------------*/
  /**
   * Specify that a new writer has been added.
   */
  virtual void addLocalWriter( int threadId ) ;

  /**
   * Specify that a writer has finished is job.
   */
  virtual void removeLocalWriter( int threadId ) ;

  /**
   * Specify that a new writer has been added.
   */
  virtual void addLocalReader( int threadId ) ;

  /**
   * Specify that a writer has finished is job.
   */
  virtual void removeLocalReader( int threadId ) ;

  /**
   * Specify that a new writer has been added.
   */
  virtual void addLocalDisplayer( int threadId ) ;

  /**
   * Specify that a writer has finished is job.
   */
  virtual void removeLocalDisplayer( int threadId ) ;

  /**
  * To know if a thread is the only writting thread
  */
  virtual bool isOnlyWriter(int threadId);

  /**
   * To know if a thread is the only displayer.
   */
  virtual bool isOnlyDisplayer(int threadId);

  /**
   * To know if a thread is the only reader.
   */
  virtual bool isOnlyReader(int threadId);
  /*-----------------------------------------------------------------------------*/
  /* Driver dependent routines */

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
   * Get the current threadId
   */
  virtual int getCurrentThreadId(void);
  /*-----------------------------------------------------------------------------*/
  std::list<int> m_oLocalReadersIds;
  std::list<int> m_oLocalWritersIds;
  std::list<int> m_oLocalDisplayersIds;

  /** Bridge for driver dependent routines */
  GraphicSynchronizerBridge * m_pBridge;
  /*-----------------------------------------------------------------------------*/

};

}

#endif /* _GLOBAL_SYNCHRONIZER_H_ */
