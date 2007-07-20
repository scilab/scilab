/*------------------------------------------------------------------------*/
/* file: GlobalSynchronizer.h                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Synchronizer able to protect the whole graphic data             */
/*------------------------------------------------------------------------*/

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
  bool areDataWritable( void ) ;

  /**
   * To know if local synchronizer can write data
   */
  bool areDataReadable( void ) ;

  /**
   * To know if local synchronizer can write data
   */
  bool areDataDisplayable( void ) ;

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
   * To know if the object can currently be written or we should wait
   */
  virtual bool isWritable( void ) ;

  /**
   * To know if the object can be read of if we need to wait
   */
  virtual bool isReadable( void ) ;

  /**
   * To know if the object can be displayed or if we need to wait
   */
  virtual bool isDisplayable( void ) ;

  /**
   * Specify that a new writer has been added.
   */
  virtual void addLocalWriter( void ) ;

  /**
   * Specify that a writer has finished is job.
   */
  virtual void removeLocalWriter( void ) ;

  /**
   * Specify that a new writer has been added.
   */
  virtual void addLocalReader( void ) ;

  /**
   * Specify that a writer has finished is job.
   */
  virtual void removeLocalReader( void ) ;

  /**
   * Specify that a new writer has been added.
   */
  virtual void addLocalDisplayer( void ) ;

  /**
   * Specify that a writer has finished is job.
   */
  virtual void removeLocalDisplayer( void ) ;
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
  /*-----------------------------------------------------------------------------*/
  /** Number of local synchronizer writing graphic data */
  int m_iNbLocalWriters;

  /** Number of local synchronizer writing graphic data */
  int m_iNbLocalReaders;

  /** Number of local synchronizer writing graphic data */
  int m_iNbLocalDisplayers;

  /** Bridge for driver dependent routines */
  GraphicSynchronizerBridge * m_pBridge;
  /*-----------------------------------------------------------------------------*/

};

}

#endif /* _GLOBAL_SYNCHRONIZER_H_ */
