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

#include "graphicSession.h"

/* boolean variable to know wether a session is opened */
static BOOL isSessionOpened = FALSE ;

/*--------------------------------------------------------------------------*/
int openGraphicSession( void )
{
  if ( !isSessionOpened )
  {
     isSessionOpened = TRUE ;
     return 0 ;
  }
  else
  {
    return 1 ;
  }
  return -1 ;
}
/*--------------------------------------------------------------------------*/
int closeGraphicSession( void )
{
  if ( isSessionOpened )
  {
    isSessionOpened = FALSE ;
    return 0 ;
  }
  else
  {
    return 1 ;
  }
  return -1 ;
}
/*--------------------------------------------------------------------------*/
BOOL isGraphicSessionOpened( void )
{
  return isSessionOpened ;
}
/*--------------------------------------------------------------------------*/
