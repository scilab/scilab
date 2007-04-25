/*------------------------------------------------------------------------*/
/* file: HandleObserver.h                                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Observer to automatically draw graphic handles                  */
/*------------------------------------------------------------------------*/

#ifndef _HANDLE_DRAWING_OBSERVER_H_
#define _HANDLE_DRAWING_OBSERVER_H_

#include "HandleObserver.h"

/**
 * Class observing handles (sciPointObj *).
 */
class HandleDrawingObserver : public HandleObserver
{
public:

  HandleDrawingObserver( sciPointObj * subject ) : HandleObserver( subject ) {}

  /**
   * Here the update action is to redraw the graphic handle
   */
  void update( void ) ;
  
} ;

#endif /* _HANDLE_DRAWING_OBSERVER_H_ */
