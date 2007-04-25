/*------------------------------------------------------------------------*/
/* file: graphicSession.h                                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : routines to open and close graphic sessions. This is used to    */
/*        prevent from opening more than one.                             */
/*------------------------------------------------------------------------*/

#ifndef _GRAPHIC_SESSION_H_
#define _GRAPHIC_SESSION_H_

#include "machine.h"

/**
 * Specify that a graphic session has been opened.
 * @return * 0  if a session has been opened
 *         * 1  if another session was already running
 *         * -1 if an other error occured.
 */
int openGraphicSession( void ) ;

/**
 * Specify that the graphic session is not used any longer.
 * @return * 0  if a session has been closed.
 *         * 1  if their was not any session opened.
 *         * -1 if an error occured.
 */
int closeGraphicSession( void ) ;

/**
 * Use to know if a graphic is opened.
 */
BOOL isGraphicSessionOpened( void ) ;


#endif /* _GRAPHIC_SESSION_H_ */
