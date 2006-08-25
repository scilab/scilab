/*------------------------------------------------------------------------*/
/* file: WindowList.c                                                     */
/* Copyright INRIA 2006                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : routine to access and modify the window list (The_List)         */
/*        routines taken from periX11 and periWin                         */
/*------------------------------------------------------------------------*/

#include "WindowList.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "core_math.h"

/**
 * list of all graphic windows
 */
static WindowList * The_List = NULL ;

/*---------------------------------------------------------------------------------*/
BOOL isWindowListEmpty( void )
{
  return The_List == NULL ;
}
/*---------------------------------------------------------------------------------*/
WindowList * getScilabWindowList( void )
{
  return The_List ;
}
/*---------------------------------------------------------------------------------*/
struct BCG * getFirstWindow( void )
{
  return &(The_List->winxgc) ;
}
/*---------------------------------------------------------------------------------*/
struct BCG * addWindowItem( void )
{
  WindowList * newItem = MALLOC( sizeof( WindowList ) ) ;

  if ( newItem == NULL )
  {
    Scierror(999,"No more memory for allocating new window\n." ) ;
    return NULL ;
  }

  newItem->next = NULL ;

  if ( The_List == NULL )
  {
    The_List = newItem ;
  }
  else
  {
    WindowList * lastItem = getLastWindowItem() ;
    lastItem->next = newItem ;
  }
  return &(newItem->winxgc) ;
}
/*---------------------------------------------------------------------------------*/
int removeWindowItem( struct BCG * window )
{
  WindowList * removedItem  = NULL ;
  WindowList * previousItem = NULL ;
  
  findWindowItem( window, &removedItem, &previousItem ) ;

  if ( removedItem == NULL )
  {
    return -1 ;
  }

  if ( previousItem == NULL )
  {
    /* removedItem is the first in the list */
    The_List = removedItem->next ;
  }
  else
  {
    previousItem->next = removedItem->next ;
  }

  FREE( removedItem ) ;
  return 0 ;
  

}
/*---------------------------------------------------------------------------------*/
struct BCG * getWindowXgcNumber( integer i )
{
  WindowList * curItem = The_List ;
  while( curItem != NULL )
  {
    if ( curItem->winxgc.CurWindow == i )
    {
      return &(curItem->winxgc) ;
    }
    curItem = curItem->next ;
  }
  return NULL ;
}
/*---------------------------------------------------------------------------------*/
void getWins( integer * Num, integer * Ids, integer * flag )
{
  WindowList * listptr = The_List ;
  *Num = 0 ;
  if ( *flag == 0 )
  {
    while ( listptr != NULL ) 
    {
      (*Num)++;
      listptr = listptr->next ;
    }
  }
  else 
  {
    while ( listptr != NULL ) 
    {
      Ids[*Num] = listptr->winxgc.CurWindow;
      (*Num)++;
      listptr = listptr->next;
    }
  }
}
/*---------------------------------------------------------------------------------*/
int getWinsMaxId( void )
{
  WindowList *listptr = The_List;
  int Num = -1;
  while ( listptr != NULL ) 
  {
    Num = Max( listptr->winxgc.CurWindow, Num ) ;
    listptr = listptr->next;
  }
  return(Num);
}
/*---------------------------------------------------------------------------------*/
WindowList * getLastWindowItem( void )
{
  WindowList * curItem = The_List ;
  while ( curItem != NULL && curItem->next != NULL )
  {
    curItem = curItem->next ;
  }
  return curItem ;
}
/*---------------------------------------------------------------------------------*/
void findWindowItem( struct BCG * window, WindowList ** item, WindowList ** previous )
{
  *item     = The_List ;
  *previous = NULL     ;
  
  if ( *item == NULL ) { return ; }

  while( *item != NULL )
  {
    if( &((*item)->winxgc) == window )
    {
      /* found!!! */
      return ;
    }
    *previous = *item ;
    *item = (*item)->next ;
  }

  *previous = NULL ;

}
/*---------------------------------------------------------------------------------*/
