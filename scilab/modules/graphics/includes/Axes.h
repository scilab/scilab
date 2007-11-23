/*======================================================================================*/
/* Copyrigth INRIA 2006                                                                 */
/* File   : Axes.h                                                                      */
/* Author : Jean-Baptiste Silvy INRIA                                                   */
/* Desc   : Various functions to manipulate SubWindow objects                           */
/*======================================================================================*/

#ifndef __SCI_AXES_H__
#define __SCI_AXES_H__

#include "ObjectStructure.h"


/*-----------------------------------------------------------------------------*/

/* clear a subwindow from all of its children */
void clearSubWin( sciPointObj * pSubWin ) ;

/* reinit a subwindow (but don't change position) */
void reinitSubWin( sciPointObj * pSubWin ) ;

/* reinit the viewing angles of a subwindow */
void initSubWinAngles( sciPointObj * pSubWin ) ;

/* set the size and position of the subwindow to the default */
void initSubWinSize( sciPointObj * pSubWin ) ;

/* set the data_bounds of the axes to the default value */
void initSubWinBounds( sciPointObj * pSubWin ) ;

/* reinit the selected subwindow if the auto_clear property is set to on */
/* return TRUE if the window has been redrawn */
BOOL checkRedrawing( void ) ;

/*-----------------------------------------------------------------------------*/
#endif /*__SCI_AXES_H_*/
