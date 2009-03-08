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
   \file canimxy3d.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief CANIMXY3D is a scope in 3D which draw its input as a XY scope, there is animation.
   \see CANIMXY3D.sci in macros/scicos_blocks/Sinks/
*/
#include "CurrentObjectsManagement.h"
#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "scicos_block4.h"
#include "DrawingBridge.h"

/** \fn canimxy3d_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
    \brief Function to draw or redraw the window
*/
void canimxy3d_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
{
  int i; //As usual
  int * ipar; //Integer Parameters
  int color_number; //Flag on Color
  int * color;
  int * line_size;
  int nbr_curves;
  int animed;
  int win; //Windows ID : To give a name to the window
  int buffer_size; //Buffer Size
  int win_pos[2]; //Position of the Window
  int win_dim[2]; //Dimension of the Window
  int nipar;
  double * rpar; //Reals parameters
  double xmin, xmax, ymin, ymax, zmin, zmax,alpha,theta; //Ymin and Ymax are vectors here
  scoGraphicalObject Pinceau; //Pointer to each polyline of each axes
  scoGraphicalObject Gomme; //Pointer to each polyline of each axes
  scoGraphicalObject Trait; //Pointer to each trache of each axes
  int number_of_subwin;
  int number_of_curves_by_subwin;
  int dimension = 3;
  int gomme_color;
  int size=0;

  ipar = GetIparPtrs(block);
  nipar = GetNipar(block);
  rpar = GetRparPtrs(block);
  win = ipar[0];
  color_number = ipar[1];
  buffer_size = ipar[2];
  color = (int*)scicos_malloc(color_number*sizeof(int));
  line_size = (int*)scicos_malloc(color_number*sizeof(int));
  for(i = 0 ; i < color_number ; i++)
    {
      color[i] = ipar[i+3];
      line_size[i] = ipar[i+3+color_number];
    }
  size = 2*color_number;
  animed = ipar[size+3];
  win_pos[0] = ipar[size+4];
  win_pos[1] = ipar[size+5];
  win_dim[0] = ipar[size+6];
  win_dim[1] = ipar[size+7];
  xmin = rpar[0];
  xmax = rpar[1];
  ymin = rpar[2];
  ymax = rpar[3];
  zmin = rpar[4];
  zmax = rpar[5];
  alpha = rpar[6];
  theta = rpar[7];
  number_of_subwin = 1;
  nbr_curves = ipar[size+8];


  /* If only one element to draw*/
  if (buffer_size == 1)
    {
      number_of_curves_by_subwin = nbr_curves;
      if(firstdraw == 1)
	{
	  scoInitScopeMemory(block->work,pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
	  scoSetShortDrawSize(*pScopeMemory,0,1);
	  scoSetLongDrawSize(*pScopeMemory,0,0);
	}

      scoInitOfWindow(*pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax);
      if(scoGetScopeActivation(*pScopeMemory) == 1)
	{ 
	  sciSetPixmapMode(scoGetPointerScopeWindow(*pScopeMemory),TRUE);
	  pFIGURE_FEATURE(scoGetPointerScopeWindow(*pScopeMemory))->pixmapMode = 1;

	  for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(*pScopeMemory, 0) ; i++)
	    {
	      scoAddPolylineForShortDraw(*pScopeMemory,0,i,color[i]);
	      Pinceau = scoGetPointerShortDraw(*pScopeMemory,0,i);

	      sciSetMarkSize(Pinceau, line_size[i]);

	      pPOLYLINE_FEATURE(Pinceau)->n1 = 1;
	    }
	}
    }
  /*else if 2 or more elements*/
  else
    {
      number_of_curves_by_subwin = 2*nbr_curves; //it is a trick to recognize the type of scope, not sure it is a good way because normally a curve is the combination of a short and a longdraw
      if(firstdraw == 1)
	{
	  scoInitScopeMemory(block->work,pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
	}
      scoInitOfWindow(*pScopeMemory, dimension, win, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax);
      if(scoGetScopeActivation(*pScopeMemory) == 1)
	{
	  gomme_color = sciGetBackground(scoGetPointerAxes(*pScopeMemory,0));

          if(firstdraw == 1) {
            scoSetShortDrawSize(*pScopeMemory,0,2);
            scoSetLongDrawSize(*pScopeMemory,0,buffer_size);
          }

          for(i = 0 ; i < nbr_curves ; i++) {
            /*if mark style*/
            if (color[i]<=0) {
              //because of color[0] is negative it will add a black mark with style number color[0]
              scoAddPolylineForShortDraw(*pScopeMemory,0,i,color[i]);
              scoAddPolylineForShortDraw(*pScopeMemory,0,i+nbr_curves,color[i]); //same type of mark and black for the rubber
              scoAddPolylineForLongDraw(*pScopeMemory,0,i,color[i]);

              Pinceau = scoGetPointerShortDraw(*pScopeMemory,0,i);
              Gomme = scoGetPointerShortDraw(*pScopeMemory,0,i+nbr_curves);
              Trait = scoGetPointerLongDraw(*pScopeMemory,0,i);

              sciSetMarkSize(Pinceau, line_size[i]);
              sciSetMarkSize(Gomme, line_size[i]);
              sciSetMarkSize(Trait, line_size[i]);

              pPOLYLINE_FEATURE(Pinceau)->n1 = 1;
              pPOLYLINE_FEATURE(Gomme)->n1 = 1;
              sciSetMarkForeground(Gomme, gomme_color); //here the rubber becomes colored like the background of the axes
              pPOLYLINE_FEATURE(Trait)->n1 = buffer_size-1;
            }
            /*if line style*/
            else {
              scoAddPolylineForShortDraw(*pScopeMemory,0,i,color[i]);
              scoAddPolylineForShortDraw(*pScopeMemory,0,i+nbr_curves,gomme_color);
              scoAddPolylineForLongDraw(*pScopeMemory,0,i,color[i]);

              Pinceau = scoGetPointerShortDraw(*pScopeMemory,0,i);
              Gomme = scoGetPointerShortDraw(*pScopeMemory,0,i+nbr_curves);
              Trait = scoGetPointerLongDraw(*pScopeMemory,0,i);

              sciSetLineWidth(Pinceau, line_size[i]);
              sciSetLineWidth(Gomme, line_size[i]);
              sciSetLineWidth(Trait, line_size[i]);

              pPOLYLINE_FEATURE(Pinceau)->n1 = 2;
              pPOLYLINE_FEATURE(Gomme)->n1 = 2;
              pPOLYLINE_FEATURE(Trait)->n1 = buffer_size;
            }
          }

	}
    }
  if(scoGetScopeActivation(*pScopeMemory) == 1)
    {
      pSUBWIN_FEATURE(scoGetPointerAxes(*pScopeMemory,0))->alpha = alpha;
      pSUBWIN_FEATURE(scoGetPointerAxes(*pScopeMemory,0))->theta = theta;

      scoAddTitlesScope(*pScopeMemory,"x","y","z");
    }
  scicos_free(color);
  scicos_free(line_size);
}
/** \fn void canimxy3d(scicos_block * block, int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
void canimxy3d(scicos_block * block, int flag)
{
  /* Declarations*/
  double *u1,*u2,*u3;
  int i;
  ScopeMemory * pScopeMemory;
  scoGraphicalObject pLongDraw;
  /* State Machine Control */
  switch(flag)
    {
    case Initialization:
      {	
	
	canimxy3d_draw(block,&pScopeMemory,1);
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
		canimxy3d_draw(block,&pScopeMemory,0);
	      }

	    /*Retrieve Elements*/
	    u1 = GetRealInPortPtrs(block,1);
	    u2 = GetRealInPortPtrs(block,2);
	    u3 = GetRealInPortPtrs(block,3);

	    scoDrawScopeAnimXYStyle(pScopeMemory,u1,u2,u3);
	  }
	break; //Break of the switch don t forget it !
      }//End of stateupdate
      
      //This case is activated when the simulation is done or when we close scicos
    case Ending:
      {
				scoRetrieveScopeMemory(block->work, &pScopeMemory);
				if(scoGetScopeActivation(pScopeMemory) == 1)
				{
					/* sciSetUsedWindow(scoGetWindowID(pScopeMemory)); */
					/* Check if figure is still opened, otherwise, don't try to destroy it again. */
					scoGraphicalObject figure = scoGetPointerScopeWindow(pScopeMemory);
					if (figure != NULL)
					{
						if(scoGetLongDrawSize(pScopeMemory,0) == 0)
						{
							for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ; i++)
							{
								pLongDraw = scoGetPointerLongDraw(pScopeMemory,0,i);
								forceRedraw(pLongDraw);
							}
						}
						else
						{
							for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0)/2 ; i++)
							{
								pLongDraw = scoGetPointerLongDraw(pScopeMemory,0,i);
								forceRedraw(pLongDraw);
							}
						}

						/* pShortDraw = sciGetCurrentFigure(); */
						/*pShortDraw = scoGetPointerScopeWindow(pScopeMemory);*/
						/*pFIGURE_FEATURE(pShortDraw)->user_data = NULL;*/
						/*pFIGURE_FEATURE(pShortDraw)->size_of_user_data = 0;*/
						clearUserData(figure);
					}
				}
				scoFreeScopeMemory(block->work, &pScopeMemory);
				break; //Break of the switch
      }
      //free the memory which is allocated at each turn by some variables
 
    }
}
