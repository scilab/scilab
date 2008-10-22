/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Synchronizer able to protect some part of the graphic data
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _LOCAL_SYNCHRONIZER_H_
#define _LOCAL_SYNCHRONIZER_H_

#include "GraphicSynchronizer.h"
#include "GlobalSynchronizer.h"

namespace sciGraphics
{

class LocalSynchronizer : public GraphicSynchronizer
{

  friend class GraphicSynchronizerFactory;

public:

  virtual ~LocalSynchronizer( void ) ;

  /**
   * Set the parent Synchronizer which may lock the whole data
   */
  void setParentSynchronizer( GlobalSynchronizer * synchronizer ) { m_pParentSynchronizer = synchronizer ; }

protected:

  /**
   * Default constructor. Factory should be used instead.
   */
  LocalSynchronizer( void ) ;

  /*--------------------------------------------------------------------------------*/
  /**
   * To know if the object can currently be written or we should wait
   */
  virtual bool isWritable( int threadId ) ;

  /**
   * To know if the object can be read of if we need to wait
   */
  virtual bool isReadable( int threadId ) ;

  /**
   * To know if the object can be displayed or if we need to wait
   */
  virtual bool isDisplayable( int threadId ) ;

  /**
   * Specify that a new writer has been added.
   */
  virtual void addWriter( int threadId ) ;

  /**
   * Specify that a writer has finished is job.
   */
  virtual void removeWriter( int threadId ) ;

  /**
   * Specify that a new writer has been added.
   */
  virtual void addReader( int threadId ) ;

  /**
   * Specify that a writer has finished is job.
   */
  virtual void removeReader( int threadId ) ;

  /**
   * Specify that a new writer has been added.
   */
  virtual void addDisplayer( int threadId ) ;

  /**
   * Specify that a writer has finished is job.
   */
  virtual void removeDisplayer( int threadId ) ;
  /*--------------------------------------------------------------------------------*/
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
  /*--------------------------------------------------------------------------------*/
  /**
   * Get a pointer on the parent synchronizer to know if graphic data are locked
   * and specify that some part of the data is locked.
   */
  GlobalSynchronizer * m_pParentSynchronizer;
  /*--------------------------------------------------------------------------------*/


};

}

#endif /* _LOCAL_SYNCHRONIZER_H_ */
