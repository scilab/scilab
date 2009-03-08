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
/**
   \file cscopxy.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief CSCOPXY is a scope in 2D which draw its input as a XY scope, there is no animation, everything is keep in memory instead of CANIMXY
   \see CSCOPXY.sci in macros/scicos_blocks/Sinks/
*/
#include "CurrentObjectsManagement.h"
#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "scicos_block4.h"
#include "DrawingBridge.h"
#include "SetJavaProperty.h"

/** \fn cscopxy_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
    \brief Function to draw or redraw the window
*/
void cscopxy_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
{
 int * ipar; //Integer Parameters
  int color_flag; //Flag on Color
  int color[2];
  int line_size;
  int animed;
  int win; //Windows ID : To give a name to the window
  int buffer_size; //Buffer Size
  int win_pos[2]; //Position of the Window
  int win_dim[2]; //Dimension of the Window
  int nipar;
  double * rpar; //Reals parameters
  double xmin, xmax, ymin, ymax; //Ymin and Ymax are vectors here
  int number_of_subwin;
  int number_of_curves_by_subwin;
  int dimension = 2;
  int i;
  scoGraphicalObject ShortDraw;
  scoGraphicalObject LongDraw;

  ipar = GetIparPtrs(block);
  nipar = GetNipar(block);
  rpar = GetRparPtrs(block);
  win = ipar[0];
  color_flag = ipar[1];
  buffer_size = ipar[2];
  color[0] = ipar[3];
  color[1] = ipar[3];
  line_size = ipar[4];
  animed = ipar[5];
  win_pos[0] = ipar[6];
  win_pos[1] = ipar[7];
  win_dim[0] = ipar[8];
  win_dim[1] = ipar[9];
  xmin = rpar[0];
  xmax = rpar[1];
  ymin = rpar[2];
  ymax = rpar[3];

  number_of_subwin = 1;
  number_of_curves_by_subwin = ipar[10]; //it is a trick to recognize the type of scope, not sure it is a good way because normally a curve is the combination of a short and a longdraw
  if(firstdraw == 1)
    {
      scoInitScopeMemory(block->work,pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
      scoSetShortDrawSize(*pScopeMemory,0,buffer_size);
      scoSetLongDrawSize(*pScopeMemory,0,5000);
    }

  scoInitOfWindow(*pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, NULL, NULL);
  if(scoGetScopeActivation(*pScopeMemory) == 1)
    {
      for( i = 0 ; i < number_of_curves_by_subwin ; i++)
	{
	  scoAddPolylineForShortDraw(*pScopeMemory,0,i,color[0]);
	  scoAddPolylineForLongDraw(*pScopeMemory,0,i,color[0]);
	  ShortDraw = scoGetPointerShortDraw(*pScopeMemory,0,i);
	  LongDraw = scoGetPointerLongDraw(*pScopeMemory,0,i);
          /* Set ShortDraw properties */
	  sciSetLineWidth(ShortDraw, line_size);
	  sciSetMarkSize(ShortDraw, line_size);
          /* Set LongDraw properties */
	  sciSetLineWidth(LongDraw, line_size);
	  sciSetMarkSize(LongDraw, line_size);
	}
      scoAddTitlesScope(*pScopeMemory,"x","y",NULL);
    }

	/* use only single buffering to be sure to draw on the screen */
	sciSetJavaUseSingleBuffer(scoGetPointerScopeWindow(*pScopeMemory), TRUE);
}


/** \fn void cscopxy(scicos_block * block, int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
void cscopxy(scicos_block * block, int flag)
{
  /* Declarations*/
  ScopeMemory * pScopeMemory;
  double *u1,*u2;
  scoGraphicalObject Pinceau; 
  int NbrPtsShort;
  int i;

  /* State Machine Control */
  switch(flag)
    {
    case Initialization:
      {
	cscopxy_draw(block,&pScopeMemory,1);
	break; //Break of the switch condition don t forget it
      } //End of Initialization

    case StateUpdate:
      {
	scoRetrieveScopeMemory(block->work,&pScopeMemory);
	if(scoGetScopeActivation(pScopeMemory) == 1)
	  {
	    
	/* Charging Elements */
	if (scoGetPointerScopeWindow(pScopeMemory) == NULL) // If the window has been destroyed we recreate it
	  {
	    cscopxy_draw(block,&pScopeMemory,0);
	  }

	u1 = GetRealInPortPtrs(block,1);
	u2 = GetRealInPortPtrs(block,2);
	
	for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0); i++)
	  {
	    Pinceau = scoGetPointerShortDraw(pScopeMemory,0,i);
	    NbrPtsShort = pPOLYLINE_FEATURE(Pinceau)->n1;
	    pPOLYLINE_FEATURE(Pinceau)->pvx[NbrPtsShort] = u1[i];
	    pPOLYLINE_FEATURE(Pinceau)->pvy[NbrPtsShort] = u2[i];
	    pPOLYLINE_FEATURE(Pinceau)->n1++;
	  }
	
	scoDrawScopeXYStyle(pScopeMemory);
	  }
	break; //Break of the switch don t forget it !
      }//End of stateupdate
      
      //This case is activated when the simulation is done or when we close scicos
    case Ending:
      {
	scoRetrieveScopeMemory(block->work, &pScopeMemory);
	if(scoGetScopeActivation(pScopeMemory) == 1)
	  {
	    /*sciSetUsedWindow(scoGetWindowID(pScopeMemory));*/
	    
			/* Check if figure is still opened, otherwise, don't try to destroy it again. */
			scoGraphicalObject figure = scoGetPointerScopeWindow(pScopeMemory);
			if (figure != NULL)
			{
				for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0); i++)
				{
					Pinceau = scoGetPointerLongDraw(pScopeMemory,0,i);
					forceRedraw(Pinceau);
				}
			
				//Here Pinceau = Window
				/*Pinceau = sciGetCurrentFigure();*/
				/*pFIGURE_FEATURE(Pinceau)->user_data = NULL;
				pFIGURE_FEATURE(Pinceau)->size_of_user_data = 0;*/
				clearUserData(figure);
				/* restore double buffering */
				sciSetJavaUseSingleBuffer(figure, FALSE);
			}
	}
	scoFreeScopeMemory(block->work, &pScopeMemory);
	break; //Break of the switch
      }
      //free the memory which is allocated at each turn by some variables
    }
}
