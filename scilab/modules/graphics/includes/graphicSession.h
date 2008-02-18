/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: graphicSession.h                                                 */
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
