/*------------------------------------------------------------------------*/
/* file: GraphicSynchronizer.h                                            */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing functions to synchronize the access to graphic */
/*        data                                                            */
/*------------------------------------------------------------------------*/

#ifndef _GRAPHIC_SYNCHRONIZER_H_
#define _GRAPHIC_SYNCHRONIZER_H_

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
  /*--------------------------------------------------------------------------------*/

protected:

  /*--------------------------------------------------------------------------------*/
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
  virtual void addWriter( void ) ;

  /**
   * Specify that a writer has finished is job.
   */
  virtual void removeWriter( void ) ;

  /**
   * Specify that a new writer has been added.
   */
  virtual void addReader( void ) ;

  /**
   * Specify that a writer has finished is job.
   */
  virtual void removeReader( void ) ;

  /**
   * Specify that a new writer has been added.
   */
  virtual void addDisplayer( void ) ;

  /**
   * Specify that a writer has finished is job.
   */
  virtual void removeDisplayer( void ) ;
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
  /*--------------------------------------------------------------------------------*/
  /** Number of active writer */
  int m_iNbWriters;
  /** Number of active reader*/
  int m_iNbReaders;
  /** Number of active displayer */
  int m_iNbDisplayers;
  /*--------------------------------------------------------------------------------*/
};

}

#endif /* _GRAPHIC_SYNCHRONIZER_H_ */
