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
   \file cmscope.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief CMSCOPE is a typical scope which links its input to the simulation time
   \see CMSCOPE.sci in macros/scicos_blocks/Sinks/
*/
/*--------------------------------------------------------------------------*/
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
/** \fn cmscope_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
    \brief Function to draw or redraw the window
*/
SCICOS_BLOCKS_IMPEXP void cmscope_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
{
  int i = 0; //As usual
  int * ipar = NULL; //Integer Parameters
  int * colors = NULL; //Colors
  int win = 0; //Windows ID : To give a name to the window
  int buffer_size = 0; //Buffer Size
  int win_pos[2]; //Position of the Window
  int win_dim[2]; //Dimension of the Window
  int inherited_events = 0;
  int nipar = 0;
  int dimension = 2;
  double * rpar = NULL; //Reals parameters
  double dt = 0.; //Time++
  double * period = NULL; //Refresh Period of the scope is a vector here
  double * ymin = NULL,* ymax = NULL; //Ymin and Ymax are vectors here
  double * xmin = NULL, *xmax = NULL;
  int nbr_period = 0;
  int * number_of_curves_by_subwin = NULL;
  int number_of_subwin = 0;
  int nbr_total_curves = 0;
  char *label = NULL;


  rpar = GetRparPtrs(block);
  ipar = GetIparPtrs(block);
  nipar = GetNipar(block);
  win = ipar[0];
  number_of_subwin = ipar[1];
  buffer_size = ipar[2];
  win_pos[0] = ipar[3];
  win_pos[1] = ipar[4];
  win_dim[0] = ipar[5];
  win_dim[1] = ipar[6];
  label = GetLabelPtrs(block);
  nbr_total_curves = 0;
  //Don't forget malloc for 'type *'
  number_of_curves_by_subwin = (int*)scicos_malloc(number_of_subwin*sizeof(int));
  for (i = 7; i < 7+number_of_subwin ; i++)
    {
      number_of_curves_by_subwin[i-7] = ipar[i];
      nbr_total_curves = nbr_total_curves + ipar[i];
    }
  colors = (int*)scicos_malloc(nbr_total_curves*sizeof(int));
  for(i = 0; i < nbr_total_curves ; i++)
    {
      colors[i] = ipar[i+7+number_of_subwin];
    }
  inherited_events = ipar[7+number_of_subwin+number_of_subwin];

  dt = rpar[0];

  nbr_period = 0;
  period = (double*)scicos_malloc(number_of_subwin*sizeof(double));
  for (i = 0 ; i < number_of_subwin ; i++)
    {
      period[i] = rpar[i+1];
      nbr_period++;
    }
  ymin = (double*)scicos_malloc(number_of_subwin*sizeof(double));
  ymax = (double*)scicos_malloc(number_of_subwin*sizeof(double));
  for (i = 0 ; i < number_of_subwin ; i++)
    {
      ymin[i] = rpar[2*i+nbr_period+1];
      ymax[i] = rpar[2*i+nbr_period+2];
    }

  /*Allocating memory*/
  if(firstdraw == 1)
    {

      scoInitScopeMemory(block->work,pScopeMemory, number_of_subwin, number_of_curves_by_subwin);
      for(i = 0 ; i < number_of_subwin ; i++)
    {
      scoSetLongDrawSize(*pScopeMemory, i, 5000);
      scoSetShortDrawSize(*pScopeMemory,i,buffer_size);
      scoSetPeriod(*pScopeMemory,i,period[i]);
    }
    }

  /* Xmin and Xmax are calculated here because we need a variable which is only existing in the pScopeMemory. pScopeMemory is allocated just few lines before. Indeed in this TimeAmplitudeScope Xmin and Xmax have to change often. To be sure to redraw in the correct scale we have to calculate xmin and xmax thanks to the period_counter. If the window haven't to be redraw (recreate)  it wouldn't be necessary*/
  xmin = (double*)scicos_malloc(number_of_subwin*sizeof(double));
  xmax = (double*)scicos_malloc(number_of_subwin*sizeof(double));
  for (i = 0 ; i < number_of_subwin ; i++)
    {
      xmin[i] = period[i]*(scoGetPeriodCounter(*pScopeMemory,i));
      xmax[i] = period[i]*(scoGetPeriodCounter(*pScopeMemory,i)+1);
    }

  /*Creating the Scope*/
  scoInitOfWindow(*pScopeMemory, dimension, win, win_pos, win_dim, xmin, xmax, ymin, ymax, NULL, NULL);
  if(scoGetScopeActivation(*pScopeMemory) == 1)
    {
      scoAddTitlesScope(*pScopeMemory,label,"t","y",NULL);

  /*Add a couple of polyline : one for the shortdraw and one for the longdraw*/
  /*     scoAddPolylineLineStyle(*pScopeMemory,colors); */
      scoAddCoupleOfPolylines(*pScopeMemory,colors);
    }
  scicos_free(number_of_curves_by_subwin);
  scicos_free(colors);
  scicos_free(period);
  scicos_free(ymin);
  scicos_free(ymax);
  scicos_free(xmin);
  scicos_free(xmax);

  /* use only single buffering to be sure to draw on the screen */
  if (scoGetPointerScopeWindow(*pScopeMemory) != NULL) {
    sciSetJavaUseSingleBuffer(scoGetPointerScopeWindow(*pScopeMemory), TRUE);
  }
}
/*--------------------------------------------------------------------------*/
/** \fn void cmscope(scicos_block * block, int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An int which indicates the state of the block (init, update, ending)
*/
SCICOS_BLOCKS_IMPEXP cmscope(scicos_block * block, int flag)
{
  /* Declarations */
  ScopeMemory * pScopeMemory = NULL;
  int NbrPtsShort = 0;
  double * u1 = NULL;
  double t = 0.; //get_scicos_time()
  scoGraphicalObject pShortDraw;
  int i = 0,j = 0;

  double d_current_real_time = 0. ;

  /* Initializations and Allocations*/
  //Allocations are done here because there are dependent of some values presents below

  /* State Machine Control */
  switch(flag)
    {
    case Initialization:
      {
    cmscope_draw(block,&pScopeMemory,1);

        //** Init the real time section
          //** current real time as double [second]
          d_current_real_time = scoGetRealTime();
          pScopeMemory->d_last_scope_update_time = d_current_real_time ; //** update the ds for the next step

    break; //Break of the switch condition: dont forget it
      } //End of Initialization

    case StateUpdate:
      {
    /*Retreiving Scope in the block->work*/
    scoRetrieveScopeMemory(block->work,&pScopeMemory);
    if(scoGetScopeActivation(pScopeMemory) == 1)
      {
        /* Charging Elements */
        t = get_scicos_time();
        /* If window has been destroyed we recreate it */
        if(scoGetPointerScopeWindow(pScopeMemory) == NULL)
          {
        cmscope_draw(block,&pScopeMemory,0);
          }

        scoRefreshDataBoundsX(pScopeMemory,t);

        //Here we are calculating the points in the polylines
        for (i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
          {
        u1 = GetRealInPortPtrs(block,i+1);
        pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,0);
        NbrPtsShort = pPOLYLINE_FEATURE(pShortDraw)->n1;
        for (j = 0; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
          {
            pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
            pPOLYLINE_FEATURE(pShortDraw)->pvx[NbrPtsShort] = t;
            pPOLYLINE_FEATURE(pShortDraw)->pvy[NbrPtsShort] = u1[j];
            pPOLYLINE_FEATURE(pShortDraw)->n1++;
          }
          }

            // Draw the Scope
        scoDrawScopeAmplitudeTimeStyle(pScopeMemory, t); //** the scope update condition
                                                             //** is hidden here

      }
    break;

      } //**End of stateupdate

   //** Ending is activated when the simulation is done or when we close Scicos
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
                /*pShortDraw = scoGetPointerScopeWindow(pScopeMemory);*/
                clearUserData(figure);

                /* restore double buffering */
                if (figure) {
                    sciSetJavaUseSingleBuffer(figure, FALSE);
                }
                scoDelCoupleOfPolylines(pScopeMemory);
            }

      }

        scoFreeScopeMemory(block->work, &pScopeMemory);

        break;
      }
    }
}
/*--------------------------------------------------------------------------*/
