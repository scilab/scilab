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
/* file: WindowList.c                                                     */
/* desc : routines which modify subwindows scale                          */
/*------------------------------------------------------------------------*/

#include "WindowList.h"
#include "GetProperty.h"
#include "MALLOC.h"

/**
 * list of all graphic windows
 */
static FigureList * sciFigureList = DoublyLinkedList_new() ;

/**
 * Return true if the window has Id winNum
 * @param[in] figure actually a (sciPointObj *)
 * @param[in] figNum actually an int *.
 */
static BOOL figureHasId(void * figure, void * figNum);

/*---------------------------------------------------------------------------------*/
BOOL sciHasFigures( void )
{
  return !List_is_empty(sciFigureList);
}
/*---------------------------------------------------------------------------------*/
FigureList * getScilabFigureList( void )
{
  return sciFigureList;
}
/*---------------------------------------------------------------------------------*/
sciPointObj * getFirstFigure( void )
{
  return (sciPointObj *) List_data(sciFigureList);
}
/*---------------------------------------------------------------------------------*/
void addNewFigureToList( sciPointObj * figure )
{
  sciFigureList = List_push(sciFigureList, figure);
}
/*---------------------------------------------------------------------------------*/
void removeFigureFromList( sciPointObj * figure )
{
  sciFigureList = List_free_item(sciFigureList, figure);
}
/*---------------------------------------------------------------------------------*/
static BOOL figureHasId(void * figure, void * figNum)
{
  return sciGetNum((sciPointObj *) figure) == *((int *) figNum);
}
/*---------------------------------------------------------------------------------*/
sciPointObj * getFigureFromIndex(int figNum)
{
  return (sciPointObj *) List_data(List_find_full(sciFigureList, &figNum, figureHasId)) ;
}
/*---------------------------------------------------------------------------------*/
BOOL sciIsExistingFigure(int figNum)
{
  return (getFigureFromIndex(figNum) != NULL);
}
/*---------------------------------------------------------------------------------*/
int sciGetNbFigure(void)
{
  return List_nb_item(sciFigureList);
}
/*---------------------------------------------------------------------------------*/
void sciGetFiguresId(int ids[])
{
  FigureList * iterator = sciFigureList;
  int i = 0;

  while( !List_is_end(sciFigureList,iterator) )
  {
    ids[i] = sciGetNum((sciPointObj *) List_data(iterator));
    i++;
    iterator = List_next(sciFigureList, iterator);
  }
}
/*---------------------------------------------------------------------------------*/
int sciGetFiguresMaxId( void )
{
  FigureList * iterator = sciFigureList;
  int res = -1;
  int i = 0;

  while( !List_is_end(sciFigureList,iterator) )
  {
    int curIndex = sciGetNum((sciPointObj *) List_data(iterator));
    if (curIndex > res) { res = curIndex; }
    i++;
    iterator = List_next(sciFigureList, iterator);
  }
  return res;
}
/*---------------------------------------------------------------------------------*/
int getUnusedFigureIndex(void)
{
  return sciGetFiguresMaxId() + 1;
}
/*---------------------------------------------------------------------------------*/
