/*------------------------------------------------------------------------*/
/* file: WindowList.h                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : routine to access and modify the window list (The_List)         */
/*        routines taken from periX11 and periWin                         */
/*------------------------------------------------------------------------*/

#ifndef _WINDOW_LIST_
#define _WINDOW_LIST_

#include "bcg.h"
#include "machine.h"

/**
 * definition of the list of windows
 */
typedef struct WindowList
{
  struct BCG winxgc ;
  struct WindowList * next ;
} WindowList  ;

/**
 * to know if there are some opened graphic windows
 */
BOOL isWindowListEmpty( void ) ;

/**
 * retrieve the scilab window list (The_List).
 */
WindowList * getScilabWindowList( void ) ;

/**
 * get the fist BCG in the list
 */
struct BCG * getFirstWindow( void ) ;

/**
 * create a new element at the end of the scilab
 * window List and return it's BCG.
 */
struct BCG * addWindowItem( void ) ;

/**
 * remove the window with a certain BCG from the list
 * @return 0  if the item was removed successfully
 *         -1 if the item has not been find.
 */
int removeWindowItem( struct BCG * window ) ;

/**
 * returns the graphic context of window i
 * or NULL if this window does not exists
 */
struct BCG * getWindowXgcNumber( integer i ) ;

/**
 * get ids of scilab windows
 * in array Ids,
 * @param[out] Num gives the number of windows
 * @param[in] flag if 1 ==> get the Ids 
 *                 if 0 ==> just get the Number Num 
 */
void getWins( integer * Num, integer * Ids, integer * flag ) ;

/**
 * get the highest Id of scilab windows
 * or -1 if no windows
 */
int getWinsMaxId( void ) ;

/*----------------------------------------------------------------------------------*/
/* Private */
/**
 * find the last item in the windowList
 */
WindowList * getLastWindowItem( void ) ;

/**
 * find an item in the windowList and its previous.
 * @param[in] window the looked for BCG
 * @param[out] item a pointer on the item. If it has not been find NULL.
 * @param[out] previous a pointer on the previous item. If the item is the first
 *                      in the list or has not been find NULL.
 */
void findWindowItem( struct BCG * window, WindowList ** item, WindowList ** previous ) ;

/*----------------------------------------------------------------------------------*/
#endif /* _WINDOW_LIST */