/*------------------------------------------------------------------------*/
/* file: Observer.h                                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : interface for observers                                         */
/*------------------------------------------------------------------------*/

#ifndef _OBSERVER_H_
#define _OBSERVER_H_


/**
 * Interface for every observer objects
 */
class Observer
{
public:

  /**
   * Function called when the subject call notify
   */
  virtual void update( void ) = 0 ;

} ;

#endif /* _OBSERVER_H_ */
