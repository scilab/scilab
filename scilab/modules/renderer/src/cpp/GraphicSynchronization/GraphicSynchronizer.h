/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing functions to synchronize the access to graphic data
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _GRAPHIC_SYNCHRONIZER_H_
#define _GRAPHIC_SYNCHRONIZER_H_

#include <list>

namespace sciGraphics
{

/**
 * Class containing functions to synchronize the access to graphic data.
 * It can be used to protect the whole graphic module or only a figure.
 */
class GraphicSynchronizer
{

public:
  /*--------------------------------------------------------------------------------*/
  /**
   * Default constructor not synchronized.
   */
  GraphicSynchronizer( void ) ;

  /**
   * destructor not synchronized.
   */
  virtual ~GraphicSynchronizer( void ) ;
  /*--------------------------------------------------------------------------------*/
  /**
   * To be called before modifying graphic data.
   * Don't forget to call enWritting when modifications are over.
   * After this call writing will begin when all other access to graphic
   * data are finished
   * When writing no other access to graphical data are allowed.
   */
  void startWriting( void ) ;

  /**
   * To be called when the data have been modified.
   * Should always be called after a startWritting call.
   */
  void endWritting( void ) ;

  /**
   * To be called before getting some graphic data.
   * Don't forget to call endReading when reading is over.
   * After this call reading will begin when every writers will finish to
   * modify graphic data.
   * When reading, writing is not allowed.
   */
  void startReading( void ) ;

  /**
   * To be called when data have been read.
   * Should always be called after a startReading call.
   */
  void endReading( void ) ;

  /**
   * To be called when displaying of the graphic data start
   * Don't forget to call endDisplaying when displaying is over.
   * After this call displaying will begin when other displaying or writing
   * thread will finish their jobs.
   * When displaying, only reading of graphical data is allowed.
   */
  void startDisplaying( void ) ;

  /**
   * To be called when data have been displayed.
   * Should always be called after a startDisplaying call.
   */
  void endDisplaying( void ) ;

  /**
   * Specify if the synchrnonizer is enable or not.
   * If enable it will stop current thread if needed.
   * If not enable, it won't stop any thread. It means
   * that the synchrnonizer can always be considered as enable to read, display and write.
   */
  void setEnable(bool enable);
  /*--------------------------------------------------------------------------------*/

protected:

  /*--------------------------------------------------------------------------------*/
  /**
  * Remove one element with value value from the list.
  * If no element is found, do nothing.
  */
  void removeOne(std::list<int>& intList, int value);
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
  /*--------------------------------------------------------------------------------*/
  /* Driver dependent routines */
  
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
   * Get the current threadId
   */
  virtual int getCurrentThreadId(void) = 0;
  /*--------------------------------------------------------------------------------*/
  /** Number of active writer */
  std::list<int> m_oReadersIds;
  /** Number of active reader*/
  std::list<int> m_oDisplayersIds;
  /** Number of active displayer */
  std::list<int> m_oWritersIds;
  /** If false, the synchrnonizer is always considered as able to read, display and write */
  bool m_bIsEnable;
  /*--------------------------------------------------------------------------------*/

};

}

#endif /* _GRAPHIC_SYNCHRONIZER_H_ */
