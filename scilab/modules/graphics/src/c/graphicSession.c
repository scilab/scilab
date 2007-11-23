/*------------------------------------------------------------------------*/
/* file: graphicSession.h                                                 */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
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
