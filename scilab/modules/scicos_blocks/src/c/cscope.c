/*  Scicos
*
*  Copyright (C) INRIA - METALAU Project <scicos@inria.fr>
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/ 
/**
   \file cscope.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief CSCOPE is a typical scope which links its input to the simulation time but there is only one input instead of CMSCOPE
   \see CSCOPE.sci in macros/scicos_blocks/Sinks/
*/
/*--------------------------------------------------------------------------*/ 
#include <stdio.h>
#include "CurrentObjectsManagement.h"
#include "scicos.h"
#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "scicos_block4.h"
#include "SetJavaProperty.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "MALLOC.h"
#include "dynlib_scicos_blocks.h"
/*--------------------------------------------------------------------------*/ 
/** \fn cscope_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
    \brief Function to draw or redraw the window
*/
SCICOS_BLOCKS_IMPEXP void cscope_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
{
  int i = 0;
  double *rpar = NULL;
  int *ipar = NULL, nipar = 0;
  double period = 0.;
  int dimension = 0;
  double ymin = 0., ymax = 0., xmin = 0., xmax = 0.;
  int buffer_size = 0;
  int win_pos[2];
  int win_dim[2];
  int win = 0;
  int number_of_subwin = 0;
  int number_of_curves_by_subwin[1];
  int * colors = NULL;
  char *label = NULL;

  /*Retrieving Parameters*/
  rpar = GetRparPtrs(block);
  ipar = GetIparPtrs(block);
  nipar = GetNipar(block);
  
  buffer_size = ipar[2]; //** this is the "standard" buffer size 
  
  win = ipar[0];
  period  = rpar[3];
  win_pos[0] = ipar[(nipar-1) - 3];
  win_pos[1] = ipar[(nipar-1) - 2];
  win_dim[0] = ipar[(nipar-1) - 1];
  win_dim[1] = ipar[nipar-1];
  dimension = 2;
  number_of_curves_by_subwin[0] = GetInPortRows(block,1);

  number_of_subwin = 1;
  ymin = rpar[1];
  ymax = rpar[2];
  label = GetLabelPtrs(block);

  colors = (int*)scicos_malloc(number_of_curves_by_subwin[0]*sizeof(int));
  for(i = 0 ; i < number_of_curves_by_subwin[0] ; i++)
    {
      colors[i] = ipar[3+i];
    }

  /*Allocating memory*/
  if(firstdraw == 1)
    {
      scoInitScopeMemory(block->work,pScopeMemory, number_of_subwin, number_of_curves_by_subwin);
      /*Must be placed before adding polyline or other elements*/
      scoSetLongDrawSize(*pScopeMemory, 0, 50);

      scoSetShortDrawSize(*pScopeMemory,0,buffer_size); //** set the short draw size 

      scoSetPeriod(*pScopeMemory,0,period);
    }

  xmin = period*scoGetPeriodCounter(*pScopeMemory,0);
  xmax = period*(scoGetPeriodCounter(*pScopeMemory,0)+1);

  /*Creating the Scope*/
  scoInitOfWindow(*pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
  if(scoGetScopeActivation(*pScopeMemory) == 1)
    {
      scoAddTitlesScope(*pScopeMemory,label,"t","y",NULL);
      /*Add a couple of polyline : one for the shortdraw and one for the longdraw*/
      scoAddCoupleOfPolylines(*pScopeMemory,colors);
      /* scoAddPolylineLineStyle(*pScopeMemory,colors); */
    }
  scicos_free(colors);

  /* use only single buffering to be sure to draw on the screen */
  if (scoGetPointerScopeWindow(*pScopeMemory) != NULL)
    {
      sciSetJavaUseSingleBuffer(scoGetPointerScopeWindow(*pScopeMemory), TRUE);
    }
  
}
/*--------------------------------------------------------------------------*/ 
/** \fn void cscope(scicos_block * block,int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
SCICOS_BLOCKS_IMPEXP void cscope(scicos_block * block,int flag)
{
  ScopeMemory * pScopeMemory = NULL;
  int i = 0;
  double t = 0.;
  int NbrPtsShort = 0;
  double * u1 = NULL;
  scoGraphicalObject pShortDraw;

  double d_current_real_time = 0.; 

  switch(flag) 
    {
    
     case Initialization:
      {
	cscope_draw(block,&pScopeMemory,1);

        //** Init the real time section
          //** current real time as double [second] 
          d_current_real_time = scoGetRealTime();
          pScopeMemory->d_last_scope_update_time = d_current_real_time ; //** update the ds for the next step
          //** printf("Init:Start! \n\n"); //** DEBUG ONLY ! 
					
	break;
      }
    
     case StateUpdate:
      {
	scoRetrieveScopeMemory(block->work,&pScopeMemory);
	if(scoGetScopeActivation(pScopeMemory) == 1)
	  {
	    t = get_scicos_time();
	    /*Retreiving Scope in the block->work*/
	    
	    /*If window has been destroyed we recreate it*/
	    if (scoGetPointerScopeWindow(pScopeMemory) == NULL)
	     {
	       cscope_draw(block,&pScopeMemory,0);
	     }
	    
            /*Maybe we are in the end of axes so we have to draw new ones */
	    scoRefreshDataBoundsX(pScopeMemory,t);

	    // Cannot be factorized depends of the scope
	    u1 = GetRealInPortPtrs(block,1);
	    for (i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ; i++)
	       {
	         pShortDraw  = scoGetPointerShortDraw(pScopeMemory,0,i);
			 if (pShortDraw)
			 {
				NbrPtsShort = pPOLYLINE_FEATURE(pShortDraw)->n1;
				pPOLYLINE_FEATURE(pShortDraw)->pvx[NbrPtsShort] = t;
				pPOLYLINE_FEATURE(pShortDraw)->pvy[NbrPtsShort] = u1[i];
				pPOLYLINE_FEATURE(pShortDraw)->n1++;
			 }
  	       }
	    // End of Cannot

	    // Draw the Scope
	    scoDrawScopeAmplitudeTimeStyle(pScopeMemory, t); //** the scope update condition
                                                             //** is hidden here 
	  }

	break;
      } //**-----------------------------------------------------------

    case Ending:
      {
	scoRetrieveScopeMemory(block->work, &pScopeMemory);
	if(scoGetScopeActivation(pScopeMemory) == 1)
	  {
	  //  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
	  //  pShortDraw = sciGetCurrentFigure();
	  //  pFIGURE_FEATURE(pShortDraw)->user_data = NULL;
	  //  pFIGURE_FEATURE(pShortDraw)->size_of_user_data = 0;
			///* restore double buffering */
			//sciSetJavaUseSingleBuffer(pShortDraw, FALSE);
	  //  scoDelCoupleOfPolylines(pScopeMemory);

			/* Check if figure is still opened, otherwise, don't try to destroy it again. */
			scoGraphicalObject figure = scoGetPointerScopeWindow(pScopeMemory);
			if (figure != NULL)
			{
				clearUserData(figure);
				/* restore double buffering */
				sciSetJavaUseSingleBuffer(figure, FALSE);
				scoDelCoupleOfPolylines(pScopeMemory);
			}

	  }
	scoFreeScopeMemory(block->work, &pScopeMemory);
	break;  
      }
    }
}
/*--------------------------------------------------------------------------*/ 
