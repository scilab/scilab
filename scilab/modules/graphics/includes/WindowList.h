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
/* file: WindowList.h                                                     */
/* desc : routine to access and modify the window list (The_List)         */
/*        routines taken from periX11 and periWin                         */
/*------------------------------------------------------------------------*/

#ifndef _WINDOW_LIST_
#define _WINDOW_LIST_

#include "machine.h"
#include "ObjectStructure.h"
#include "DoublyLinkedList.h"

/**
 * List of all scilab figure
 */
typedef DoublyLinkedList FigureList;

/**
 * to know if there are some opened graphic windows
 */
BOOL sciHasFigures( void ) ;

/**
 * retrieve the scilab window list (The_List).
 */
FigureList * getScilabFigureList( void ) ;

/**
 * get the fist BCG in the list
 */
sciPointObj * getFirstFigure( void ) ;

/**
 * create a new element at the end of the scilab
 * window List and return it's BCG.
 */
void addNewFigureToList(sciPointObj * figure);

/**
 * remove a figure from the list.
 * @return 0  if the item was removed successfully
 *         -1 if the item has not been find.
 */
void removeFigureFromList(sciPointObj * figure);

/**
 * returns the graphic context of window i
 * or NULL if this window does not exists
 */
sciPointObj * getFigureFromIndex(int figNum) ;

/**
 * @return TRUE if the figure with index id exists
 */
BOOL sciIsExistingFigure(int figNum);



/**
 * @return number of opened windows in Scilab.
 */
int sciGetNbFigure(void);

/**
 * Fill the array Ids with all the figure ids currently used by Scilab.
 * @param ids should be as long as there are figures.
 */
void sciGetFiguresId(int ids[]);

/**
 * get the highest Id of Scilab figures.
 * or -1 if no windows
 */
int sciGetFiguresMaxId( void ) ;

/**
 * @return an unused index by graphic figures. Used when creating a new window.
 */
int getUnusedFigureIndex(void);


#endif /* _WINDOW_LIST */
