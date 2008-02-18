/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2001 - ENPC - Jean-Philippe Chancelier
 * Copyright (C) 2004 - INRIA - Allan Cornet
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


/*------------------------------------------------------------------------
 *    Graphic library
 *    Copyright (C) 1998-2001 Enpc/Jean-Philippe Chancelier
 *    jpc@cermics.enpc.fr 
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "math_graphics.h"
#include "GetProperty.h"
#include "SetProperty.h"
#include "DrawObjects.h"
#include "WindowList.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "Actions.h"
#include "syncexec.h"
#include "DrawingBridge.h"
#include "CurrentObjectsManagement.h"
#include "localization.h"
#include "DrawingBridge.h"

extern int xinitxend_flag;

/********************************************************
 * the functions in this file are called from 
 * callback ( see SGraph.c ) for the XWindow version 
 * Nov 1998 : we must be sure that during the evaluation of 
 *            scig_xxx an other function scig_yyy won't be 
 *            run. This is possible since during the execution of 
 *            one scig_xxx function a sciprint can be performed 
 *            and it will lead to an event check which can 
 *            produce a call to an other scig_yyy function 
 *            flag scig_buzy  is used to check for that 
 *            
 ********************************************************/

#if !defined(_MSC_VER)
extern int WithBackingStore();
#endif
static int scig_buzy = 0;

int I3dRotation(void) ;

typedef int (*Scig_handler) __PARAMS((int));

void reset_scig_handler(void);
int scig_handler_none(int win_num);

void scig_savesg( int win_num, char * filename ) ;

/********************************************************
 * A handler which can be dynamically set to custiomize 
 * action of scig_xxx functions 
 ********************************************************/

int scig_handler_none(int win_num) {
	return win_num;
};

Scig_handler scig_handler = scig_handler_none;


/********************************************************
 * Basic Replay : redraw recorded graphics 
 ********************************************************/

void scig_replay(integer win_num)
{
  /* Modification Allan CORNET Mai 2004 */
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  sciDrawObj(getFigureFromIndex(win_num));
  scig_buzy = 0;
}


void scig_erase(integer win_num)
{
  if ( scig_buzy  == 1 ) return ;
  sciClearFigure(getFigureFromIndex(win_num));
  scig_buzy = 0;
}



int scig_2dzoom(integer win_num)
{

  if ( scig_buzy  == 1 ) return 0;
  scig_buzy =1;

  /* TODO */

  return 0;
}

void scig_unzoom(integer win_num)
{
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
 
  /* TODO */

  scig_buzy = 0;
}

int scig_3drot(integer win_num)
{
  if ( scig_buzy  == 1 ) return 0;
  scig_buzy =1;
  
  /* TODO */

  scig_buzy = 0;
  return 0;
}

void scig_sel(integer win_num)
{
  sciSwitchWindow(win_num) ;
}

void scig_loadsg(int win_num, char *filename)
{
  integer ierr ;
  integer seq = 1 ;
  int curFigId = sciGetNum(sciGetCurrentFigure());
  char * macroCall = NULL ;
  /* the sting is "xload('(1)')" where (1) is filemame */
  /* Consequently we have 9 fixed character and two variable strings. */
  int macroCallLength = 9 + (int)strlen(filename) ;

  if ( scig_buzy  == 1 ) { return ; }
  scig_buzy  = 1 ;
  
  sciSetUsedWindow(win_num);

  macroCall = MALLOC( (macroCallLength+1) * sizeof(char) ) ; /* +1 for the \0 terminating character */
  sprintf(macroCall,"xload('%s')",filename);

  C2F(syncexec)(macroCall,&macroCallLength,&ierr,&seq, macroCallLength );

  FREE( macroCall ) ;
  if(ierr != 0) { sciprint(_("Wrong plot file: %s\n"),filename) ; }

  sciSetUsedWindow(curFigId);

  scig_buzy = 0;
}

void scig_savesg( int win_num, char * filename )
{
  integer ierr ;
  integer seq = 1 ;
  char * macroCall = NULL ;

  /* the sting is "xsave('(1)',(2))" where (1) is filemame and (2) the string */
  /* corresponding of win_num. Consequently we have 10 fixed character and two variable strings. */
  int macroCallLength = 10 +  (int)strlen(filename) + GET_NB_DIGITS(win_num) ;

  macroCall = MALLOC( (macroCallLength+1) * sizeof(char) ) ; /* +1 for the \0 terminating character */

  sprintf( macroCall, "xsave('%s',%d)", filename, win_num ) ; /* call xsave macro */
  C2F(syncexec)(macroCall,&macroCallLength,&ierr,&seq, macroCallLength );
  FREE( macroCall ) ;
}

/********************************************************
* Basic Replay : expose graphics i.e 
* if we have a pixmap we can perform a wshow 
* else we perform a sgig_replay 
********************************************************/

void scig_expose(integer win_num)
{
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  
  sciDrawObj(getFigureFromIndex(win_num));
  
  scig_buzy = 0;
}

/********************************************************
* Redraw graphic window win_num  after resizing 
********************************************************/

void scig_resize(integer win_num)
{
  if ( scig_buzy  == 1 ) return ;
  scig_buzy =1;
  sciDrawObj(getFigureFromIndex(win_num));
  scig_buzy = 0;
}

/********************************************************
* graphic Window raise 
********************************************************/

void scig_raise(integer win_num)
{
  sciDrawObj(getFigureFromIndex(win_num));
}
