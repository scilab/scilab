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
   \file scoWindowScope.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief Source Code of all functions wich interact with the window like creation of graphical object or refreshing the window
*/
#include <stdio.h>
#include "scoBase.h"
#include "scoWindowScope.h"
#include "scoMemoryScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "SetProperty.h"
#include "BuildObjects.h"
#include "scicos_malloc.h"
#include "scicos_free.h"
#include "sciprint.h"
#include "WindowList.h"
#include "DrawingBridge.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"
#include "HandleManagement.h" /* sciGetHandle */

extern int C2F(dcopy)();

void scoSetWindowIDInUserData(ScopeMemory * pScopeMemory,int block_number)
{
  scoGraphicalObject pTemp = scoGetPointerScopeWindow(pScopeMemory);
  pFIGURE_FEATURE(pTemp)->user_data = (int*)scicos_malloc(sizeof(int));
  pFIGURE_FEATURE(pTemp)->user_data[0] = block_number;
  pFIGURE_FEATURE(pTemp)->size_of_user_data = 1;
}


scoInteger scoGetUserData(scoGraphicalObject pTemp)
{
  return pFIGURE_FEATURE(pTemp)->user_data[0];
}


void scoInitOfWindow(ScopeMemory * pScopeMemory, int dimension, int win_id, int * win_pos, int * win_dim, double * xmin, double * xmax, double * ymin, double * ymax, double * zmin, double * zmax)
{
  int i;
  extern int get_block_number();
  int block_number = get_block_number();
  int user_data = -1;
  scoGraphicalObject pTemp;
  scoGraphicalObject pTemp2;
  //if win-id is -1 we give an auto number based on the block_number on the diagram
  if (win_id == -1)
    {
      win_id = 20000 + block_number; 
    }
  //if we restart the simulation this command delete all previous children of the window
  /* WARNING HERE */
  /* warning C4047: '=' : 'scoGraphicalObject' differs in levels of indirection from 'BOOL' */
  if ((sciIsExistingFigure(win_id)))
    {
      pTemp = getFigureFromIndex(win_id);
      if (pFIGURE_FEATURE(pTemp)->user_data != NULL)
        {
           user_data = scoGetUserData(pTemp);
        }
    
        sciClearFigure(pTemp); //** if already present is cleared 

    }
  /*if ((user_data == -1 ) || (user_data == win_id))*/
  if ((user_data == -1 ) || (user_data == block_number))
    {
      scoSetWindowID(pScopeMemory,win_id);
      
      scoSetScopeActivation(pScopeMemory,1); //Activate It ! Let's Rock !
      //Dont forget this kind of command
      sciSetUsedWindow(scoGetWindowID(pScopeMemory));
      pTemp = sciGetCurrentFigure();
      scoSetHandleScopeWindow(pScopeMemory,sciGetHandle(pTemp));
      scoSetWindowIDInUserData(pScopeMemory,block_number);
      for (i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
        {
          if (i == 0)
            {
              //We are getting the Handle of the current axes but in the same time we are constructing it (see below)
              //Here pTemp is the pointer on the ScopeWindow
                 // J-B please check this 
                        scoSetHandleAxes(pScopeMemory,i,sciGetHandle(sciGetFirstTypedSelectedSon( pTemp, SCI_SUBWIN )) );
            }
          else
            {
              //Here pTemp is the pointer on the ScopeWindow
              scoSetHandleAxes(pScopeMemory,i,sciGetHandle(ConstructSubWin(pTemp)));
            }
          //Here pTemp2 is the pointer on the current Axes
          pTemp2 = scoGetPointerAxes(pScopeMemory,i);
          sciInitFontSize(pTemp2, 1); //** axes font size (numbers)
          
          //** sciSetIsBoxed(pTemp2,TRUE); //** obsolete in Scilab 5
          sciSetBoxType(pTemp2,BT_ON);

          //Here we don't want "smart" limits
          pSUBWIN_FEATURE(pTemp2)->tight_limits = TRUE;
          //Here PTemp2 is the pointer on the current Subwint
          //WRect is for position of Axes in the window
          pSUBWIN_FEATURE(pTemp2)->WRect[0] = 0;
          pSUBWIN_FEATURE(pTemp2)->WRect[1] = (double)i/scoGetNumberOfSubwin(pScopeMemory);
          pSUBWIN_FEATURE(pTemp2)->WRect[2] = 1;
          pSUBWIN_FEATURE(pTemp2)->WRect[3] = (double)1/scoGetNumberOfSubwin(pScopeMemory);
					pSUBWIN_FEATURE(pTemp2)->axes.filled = FALSE;
          switch(dimension)
            {
            case 3:
              pSUBWIN_FEATURE(pTemp2)->is3d = TRUE;
              pSUBWIN_FEATURE(pTemp2)->axes.axes_visible[2] = TRUE;
              //SRECT is here to give number of x,y, or z legends
              pSUBWIN_FEATURE(pTemp2)->SRect[4] = zmin[i];
              pSUBWIN_FEATURE(pTemp2)->SRect[5] = zmax[i];
            case 2:
              pSUBWIN_FEATURE(pTemp2)->axes.axes_visible[1] = TRUE;
              pSUBWIN_FEATURE(pTemp2)->SRect[2] = ymin[i];
              pSUBWIN_FEATURE(pTemp2)->SRect[3] = ymax[i];
            case 1:
              pSUBWIN_FEATURE(pTemp2)->axes.axes_visible[0] = TRUE;
              pSUBWIN_FEATURE(pTemp2)->SRect[0] = xmin[i];
              pSUBWIN_FEATURE(pTemp2)->SRect[1] = xmax[i];
              break;
            default:
              sciprint("SCOPE ERROR : Error in dimension number\n");
              break;
            }
	  forceRedraw(pTemp2);

        }
      if(win_pos != NULL)
        {
                if (win_pos[0] >= 0) {
                        sciInitScreenPosition(pTemp, win_pos[0], win_pos[1]);
                }
        }
      if(win_dim != NULL)
        {
          if (win_dim[0] >= 0) {
            /*sciSetDim(pTemp, &win_dim[0], &win_dim[1]);
            sciSetDim(pTemp2, &win_dim[0], &win_dim[1]);*/
                  sciSetDimension(pTemp, win_dim[0], win_dim[1]);
                  /*sciSetDimension(pTemp2,win_dim[0], win_dim[1]);*/
          }
        }

      sciSetUsedWindow(scoGetWindowID(pScopeMemory));
      sciDrawObj(pTemp);
    }
  else
    {
      sciprint("This Scope (block number : %d) has same number than another. It has been desactivated !\n",block_number);
    }
}

void scoRefreshDataBoundsX(ScopeMemory * pScopeMemory, double t)
{
  scoGraphicalObject pLongDraw;
  scoGraphicalObject pShortDraw;
  scoGraphicalObject pAxes;

  int NbrPts, current_period_counter;
  int i,j;
  int c__1 =   1 ;
  /* To know if one or more subwindows need to be redrawn */
  BOOL needRedraw = FALSE;
  double period;
  /*Go on the window*/
  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
  for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      /*if we have to redraw the axis for x-axis*/
      if (scoGetNewDraw(pScopeMemory,i) < 0)
        {
          needRedraw = TRUE;
          pAxes = scoGetPointerAxes(pScopeMemory,i);
          period = scoGetPeriod(pScopeMemory,i);

          /*Calculating the current period counter and update the value of x bounds*/
          current_period_counter = (int)(t/period);
          pSUBWIN_FEATURE(pAxes)->SRect[0] = period*(current_period_counter);
          pSUBWIN_FEATURE(pAxes)->SRect[1] = period*(current_period_counter+1);
	  /*forceRedraw(pAxes);*/
          /*Don't forget to save the new value - because we have activated the refresh we are going one step over*/
          scoSetPeriodCounter(pScopeMemory,i,current_period_counter);
          
          /*Instruction to reinitialize the ShortDraw and to copy the last points of the ShortDraw in the LongDraw to be sure to have continuity*/
          pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,0);
          switch(sciGetEntityType(pShortDraw))
            {
              /*Getting Number of Points in the ShortDraw
                Then for each polyline in the axis :
                -Reinitialize LongDraw
                -Copy last points of the ShortDraw into the LongDraw
                -Initialize LongDraw
                -Reinitialize the ShortDraw
                -copy last points of the last short into the new for continuity
                -initialize shortdraw
              */
            case SCI_POLYLINE:
              {
                NbrPts = pPOLYLINE_FEATURE(pShortDraw)->n1;
                for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
                  {
                    pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
                    pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
                    //We don't have to draw it because for instance there will be only one point and if it is a line it is not good
                    //** pPOLYLINE_FEATURE(pShortDraw)->visible = FALSE;

                    pPOLYLINE_FEATURE(pLongDraw)->n1 = 0;
                    /* We copy previous values to ensure the continuity */
                    C2F(dcopy)(&NbrPts,pPOLYLINE_FEATURE(pShortDraw)->pvx,&c__1,pPOLYLINE_FEATURE(pLongDraw)->pvx,&c__1);
                    C2F(dcopy)(&NbrPts,pPOLYLINE_FEATURE(pShortDraw)->pvy,&c__1,pPOLYLINE_FEATURE(pLongDraw)->pvy,&c__1);
                    pPOLYLINE_FEATURE(pLongDraw)->n1 = NbrPts;
                    
                    /* The next starting point */
                    pPOLYLINE_FEATURE(pShortDraw)->pvx[0] = pPOLYLINE_FEATURE(pLongDraw)->pvx[NbrPts-1];
                    pPOLYLINE_FEATURE(pShortDraw)->pvy[0] = pPOLYLINE_FEATURE(pLongDraw)->pvy[NbrPts-1];
                    pPOLYLINE_FEATURE(pShortDraw)->n1 = 1;
                  }
                break;
              }
            case SCI_SEGS:
              {
                NbrPts = pSEGS_FEATURE(pShortDraw)->Nbr1;
                for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
                  {
                    pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
                    pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
                    //** pSEGS_FEATURE(pShortDraw)->visible = FALSE;

                    pSEGS_FEATURE(pLongDraw)->Nbr1 = 0;
                    pSEGS_FEATURE(pLongDraw)->Nbr2 = 0;
                    C2F(dcopy)(&NbrPts,pSEGS_FEATURE(pShortDraw)->vx ,&c__1,pSEGS_FEATURE(pLongDraw)->vx,&c__1);
                    C2F(dcopy)(&NbrPts,pSEGS_FEATURE(pShortDraw)->vy ,&c__1,pSEGS_FEATURE(pLongDraw)->vy,&c__1);
                    pSEGS_FEATURE(pLongDraw)->Nbr1 = NbrPts;
                    pSEGS_FEATURE(pLongDraw)->Nbr2 = NbrPts;
                  }
                break;
              }
            default:
              sciprint("SCOPE ERROR : Cannot use scoRefreshDataBoundsX() with this type of object\n");
              break;
            }
          /* Dont forget to reinit it */
          scoSetNewDraw(pScopeMemory,i,0);

          /* we have modified some thing it is that we need to redraw the subwin */
          forceRedraw(pShortDraw);
	  forceRedraw(pLongDraw);
	  forceRedraw(pAxes);
        }
    }

  if (needRedraw)
  {
    /* redraw all the long draws */
    /*for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
      {
        pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
        sciSetVisibility(pLongDraw, TRUE);
        forceRedraw(pLongDraw);
      }
    }*/

    sciDrawObj(scoGetPointerScopeWindow(pScopeMemory));

    /* hide them as before */
    /*for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
      {
        pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
        sciSetVisibility(pLongDraw, FALSE);
        forceRedraw(pLongDraw);
      }
    }*/
  }
  

  //Now that we have redraw the window we can reactivate the shortdraw because there will be more than one point in the futur
  //THIS PART CAN BE RECODED - NOT EFFICIENT
  for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
        {
          pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,0);
          switch(sciGetEntityType(pShortDraw))
            {
            case SCI_POLYLINE:
              {
                pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
                pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
                pPOLYLINE_FEATURE(pShortDraw)->visible = TRUE;
                break;
              }
            case SCI_SEGS:
              {
                pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
                pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
                pSEGS_FEATURE(pShortDraw)->visible = TRUE;
                break;
              }
            default:
              {
                break;
              }
            }
        }
    }
}


//** ------------------------------------------------------------------------------------------------
//**
//** ------------ The physical redraw of the scope is hidden here ----------------------------------- 
//**
//**
void scoDrawScopeAmplitudeTimeStyle(ScopeMemory * pScopeMemory, double t)
{
  int c__1 = 1;
  int i,j;
  int NbrPtsShort,NbrPtsLong,inc;
  int NbrPtsToCopy;
  int current_period_counter;
  int NumCurvInSubWin;
  int NumSubWin ;
  int NumCurvTotal;
  int ShortDrawTableIndex;  
  scoGraphicalObject pLongDraw;
  scoGraphicalObject pShortDraw;
  scoGraphicalObject *pShortDrawTable;

  double d_current_real_time ; //** the current real time as  double (52 bit) data structure
  double last_update_time , delta_time, refresh_time;
  int force_update[256]; //** ... for some headroom :)
 
       //** ------ Real Time section -------------------------------------------------------------  
       //** current real time as double [second] 
       d_current_real_time = scoGetRealTime();  
       last_update_time = pScopeMemory->d_last_scope_update_time; //** recover the last update time 
       delta_time = d_current_real_time - last_update_time ; 
       refresh_time = 0.040 ; //** 40ms -> 25 fps min  
       //** ------ End of Real Time section ------------------------------------------------------

  ShortDrawTableIndex = 0; 
  NumCurvTotal = 0;  
  NumSubWin = scoGetNumberOfSubwin(pScopeMemory);
  for(i = 0 ; i < NumSubWin ; i++) //** for all the subwindows in the scope 
    {
       pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,0);
       switch (sciGetEntityType(pShortDraw))
             {

                case SCI_POLYLINE:
                      NbrPtsShort = pPOLYLINE_FEATURE(pShortDraw)->n1; //** this is incremented by one at each iteration 
                break;

                case SCI_SEGS:
                      NbrPtsShort = pSEGS_FEATURE(pShortDraw)->Nbr1;
                break;

                default:
                      sciprint("SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()\n");
                break;
             }

       //** combined condition for forced redraw
       if ( ((delta_time>refresh_time) && (NbrPtsShort>=2)) || (NbrPtsShort>=scoGetShortDrawSize(pScopeMemory,i)) )
         { 
           //** printf("Delta Time Forced Update = %f ; buffer = %d \n", delta_time, NbrPtsShort); //** DEBUG ONLY 
           force_update[i] = 1; 
         }
       else
         { 
           force_update[i] = 0; //** do not update the subwin 
         }                  
       //** ------ End of Real Time section ------------------------------------------------------

      NumCurvTotal += scoGetNumberOfCurvesBySubwin(pScopeMemory,i);

    } 

  pShortDrawTable = scicos_malloc( NumCurvTotal * sizeof(scoGraphicalObject) );
  //** for all the sub window in the scope 
  for (i = 0 ; i < NumSubWin ; i++) 
     {
       pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,0);
       switch (sciGetEntityType(pShortDraw))
             {

                case SCI_POLYLINE:
                      NbrPtsShort = pPOLYLINE_FEATURE(pShortDraw)->n1; //** this is incremented by one at each iteration 
                break;

                case SCI_SEGS:
                      NbrPtsShort = pSEGS_FEATURE(pShortDraw)->Nbr1;
                break;

                default:
                      sciprint("SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()\n");
                break;
             }

       //**-------------------------------------------------------------------------------------------------------    
       //**  
       if (force_update[i] == 1) 
         {   
           /*Block for Realloc*/
           pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,0);
           switch (sciGetEntityType(pLongDraw))
                {
                  case SCI_POLYLINE:
                      NbrPtsLong = pPOLYLINE_FEATURE(pLongDraw)->n1;
                  break;
                  case SCI_SEGS:
                      NbrPtsLong = pSEGS_FEATURE(pLongDraw)->Nbr1;
                  break;
                  default:
                      sciprint("SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()\n");
                  break;
                }

           if ((NbrPtsLong + scoGetShortDrawSize(pScopeMemory,i)) >= scoGetLongDrawSize(pScopeMemory,i))
             {
                for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
                {
                  pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
                  scoReallocLongDraw(pLongDraw, NbrPtsLong, scoGetShortDrawSize(pScopeMemory,i),10000);
                }
               //Dont forget this one - If in the futur LongDrawSize is a table we can put it in the scoReallocLongDraw() function
               scoSetLongDrawSize(pScopeMemory,i,NbrPtsLong + scoGetShortDrawSize(pScopeMemory,i)+10000);
             }
           pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,0);
           /*End od block for Realloc*/
          
           /*Block for Draw*/
           NumCurvInSubWin = scoGetNumberOfCurvesBySubwin(pScopeMemory,i); //** get number of curves in subwin 
           for (j = 0 ; j < NumCurvInSubWin ; j++)
              {
                pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
                pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);

                if (pShortDraw != NULL) //** protection for NULL pointer 
                  {
                    sciSetUsedWindow(scoGetWindowID(pScopeMemory));
                    sciSetSelectedSubWin(scoGetPointerAxes(pScopeMemory,i));
										sciSetVisibility(pShortDraw, TRUE);
                    pShortDrawTable[ShortDrawTableIndex] = pShortDraw ;
                    ShortDrawTableIndex++;
                  }
                else
                 {
                    scoScopeError(pScopeMemory,0);
                 }
              } //** end of the for() loop
            /*End of Block for Draw*/
 
        }
    } //** end of the for loop for all the subwindow in the scope 

    /* Draw all short draw(s) in all the _concerned_ subwindows in a row :) */
    sciDrawSetOfObj(pShortDrawTable, ShortDrawTableIndex );
    scicos_free(pShortDrawTable);

    for (i = 0 ; i < NumSubWin ; i++) //** for all the subwin in the scope 
       {
         pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,0);
         pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,0);
         switch (sciGetEntityType(pShortDraw))
               {
                case SCI_POLYLINE:
                    NbrPtsShort = pPOLYLINE_FEATURE(pShortDraw)->n1; //** this is incremented by one at each iteration 
                break;
                case SCI_SEGS:
                    NbrPtsShort = pSEGS_FEATURE(pShortDraw)->Nbr1;
                break;
                default:
                    sciprint("SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()\n");
                break;
               }
         if (force_update[i] == 1) //** scope need a redraw 
         {
          switch (sciGetEntityType(pLongDraw))
          {
            case SCI_POLYLINE:
              NbrPtsShort = pPOLYLINE_FEATURE(pShortDraw)->n1-1;
              NbrPtsLong = pPOLYLINE_FEATURE(pLongDraw)->n1;
              break;
            case SCI_SEGS:
              NbrPtsShort = pSEGS_FEATURE(pShortDraw)->Nbr1;
              NbrPtsLong = pSEGS_FEATURE(pLongDraw)->Nbr1;
              break;
            default:
              sciprint("SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()\n");
              break;
          }

          for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
          {
            pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
            pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
            /*Block for Memory*/
            /*
             * Alan's patch, 04/10/2007 : add NbrPtsToCopy to copy good numbers of pts for the first buffer in LongDraw + 
             * Tricks to handle the trace empty case
             */
            inc = NbrPtsLong!=0; 
            NbrPtsToCopy = NbrPtsShort + (NbrPtsLong==0);

            switch (sciGetEntityType(pShortDraw))
            {
              case SCI_POLYLINE:
                NbrPtsToCopy = NbrPtsShort + (NbrPtsLong==0);
                // We have draw but now we have to copy values in the memory of the shortdraw
                C2F(dcopy)(&NbrPtsToCopy,pPOLYLINE_FEATURE(pShortDraw)->pvx+inc,&c__1,pPOLYLINE_FEATURE(pLongDraw)->pvx+NbrPtsLong,&c__1);
                C2F(dcopy)(&NbrPtsToCopy,pPOLYLINE_FEATURE(pShortDraw)->pvy+inc,&c__1,pPOLYLINE_FEATURE(pLongDraw)->pvy+NbrPtsLong,&c__1);
                pPOLYLINE_FEATURE(pLongDraw)->n1 = NbrPtsLong+NbrPtsToCopy;
                break;
              case SCI_SEGS:
                C2F(dcopy)(&NbrPtsShort,pSEGS_FEATURE(pShortDraw)->vx,&c__1,pSEGS_FEATURE(pLongDraw)->vx+NbrPtsLong,&c__1);
                C2F(dcopy)(&NbrPtsShort,pSEGS_FEATURE(pShortDraw)->vy,&c__1,pSEGS_FEATURE(pLongDraw)->vy+NbrPtsLong,&c__1);
                pSEGS_FEATURE(pLongDraw)->Nbr1 = NbrPtsLong+NbrPtsShort;
                pSEGS_FEATURE(pLongDraw)->Nbr2 = NbrPtsLong+NbrPtsShort;
                break;
              default:
                sciprint("SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()\n");
                break;
            }
            /*End of Block for Memory*/
          }

              /* Block for Detecting redrawing of x-axis values and others activities*/
              current_period_counter = (int)(t/scoGetPeriod(pScopeMemory,i));
              // Maybe for the axes we are at the end of it - it is here that we detect and notify it
              if (current_period_counter != scoGetPeriodCounter(pScopeMemory,i))
                {
                  scoSetNewDraw(pScopeMemory,i,-1);
                  scoSetPeriodCounter(pScopeMemory,i,current_period_counter);
                }
              else
                {
                  for (j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
                     {
                       pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
                       switch (sciGetEntityType(pShortDraw))
                             {
                               case SCI_POLYLINE:
                                   /* Last point is next first point*/
                                   pPOLYLINE_FEATURE(pShortDraw)->pvx[0] = pPOLYLINE_FEATURE(pShortDraw)->pvx[NbrPtsShort];
                                   pPOLYLINE_FEATURE(pShortDraw)->pvy[0] = pPOLYLINE_FEATURE(pShortDraw)->pvy[NbrPtsShort];
                                   pPOLYLINE_FEATURE(pShortDraw)->n1 = 1;
                               break;
                               case SCI_SEGS:
                                   /* Do Nothing */
                               break;
                               default:
                                   sciprint("SCOPE ERROR : Error in scoDrawScopeAmplitudeTimeStyle()\n");
                               break;
                             }
                     }
                } //** close of if() 

              pScopeMemory->d_last_scope_update_time = scoGetRealTime(); //** update the time stamping AFTER the draw 
              //** this "trick" assure at least 25ms of pure simulation (if the buffer is big enough)

          } //** the scope has been updated  

       } //** for all the subwin

}

//**-------------------------------------------------------------------------------------------------------------
scoGraphicalObject scoCreatePolyline(scoGraphicalObject pAxes, scoInteger polyline_size, int color)
{
  scoGraphicalObject pPolyline;
  int i;
  double * vx, * vy, *vz;

  vx = (double*)scicos_malloc(polyline_size*sizeof(double));
  vy = (double*)scicos_malloc(polyline_size*sizeof(double));

  /* Alan, 29/09/07 : initial value set to zero for buffer
   * to disable random display at the beginning of the simulation
   */
  for (i=0;i<polyline_size;i++) {
   vx[i] = 0.0;
   vy[i] = 0.0;
  }

  if (pSUBWIN_FEATURE(pAxes)->axes.axes_visible[2] == TRUE) //3D
    {
      vz = (double*)scicos_malloc(polyline_size*sizeof(double));
      /* Alan, 29/09/07 : initial values set to zero for buffer
       * to disable random display at the beginning of the simulation
       */
      for (i=0;i<polyline_size;i++) {
        vz[i] = 0.0;
      }
      pPolyline=ConstructPolyline(pAxes,vx,vy,vz,0,polyline_size,1, NULL,NULL,NULL,NULL,NULL,FALSE,FALSE,TRUE,FALSE);
      scicos_free(vz);
    }
  else //2D
    {
      pPolyline=ConstructPolyline(pAxes,vx,vy,NULL,0,polyline_size,1, NULL,NULL,NULL,NULL,NULL,FALSE,FALSE,TRUE,FALSE);
    }

  scicos_free(vx);
  scicos_free(vy);

  pPOLYLINE_FEATURE(pPolyline)->n1 = 0;
  if (color > 0)
    {
      sciSetForeground(pPolyline, color);
      sciSetIsLine(pPolyline, 1);
      sciSetLineStyle(pPolyline, 1);
      sciSetMarkStyle(pPolyline, 0);
      sciSetIsMark(pPolyline, 0);
    }
  else if (color <= 0)
    {
      sciSetMarkForeground(pPolyline, -1);
      sciSetIsLine(pPolyline, 0);
      sciSetLineStyle(pPolyline, 0);
      sciSetIsMark(pPolyline, 1);
      sciSetMarkSizeUnit(pPolyline, 1);
      if (color!=0) {
        sciSetMarkStyle(pPolyline, -color);
        sciSetMarkSize(pPolyline, 4);
      }
      else {
        sciSetMarkStyle(pPolyline, 11);
        sciSetMarkSize(pPolyline, 0);
      sciSetMarkBackground(pPolyline, -1);
      }
    }
  sciSetIsClipping(pPolyline, 0);



  return pPolyline;
}

void scoAddPolylineForShortDraw(ScopeMemory * pScopeMemory, int i, int j, int color)
{
  scoGraphicalObject pShortDraw;
  pShortDraw = scoCreatePolyline(scoGetPointerAxes(pScopeMemory,i),scoGetShortDrawSize(pScopeMemory,i),color);
  scoSetHandleFromPointerShortDraw(pScopeMemory,i,j,pShortDraw);
}

void scoAddPolylineForLongDraw(ScopeMemory * pScopeMemory, int i, int j, int color)
{

  scoGraphicalObject pLongDraw;
  pLongDraw = scoCreatePolyline(scoGetPointerAxes(pScopeMemory,i),scoGetLongDrawSize(pScopeMemory,i),color);
  scoSetHandleFromPointerLongDraw(pScopeMemory,i,j,pLongDraw);

}

void scoAddCoupleOfPolylines(ScopeMemory * pScopeMemory, int * colors)
{
  int i,j;
  int inc = -1;
  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
  for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      for(j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
        {
          if(colors != NULL)
            {
              scoAddPolylineForShortDraw(pScopeMemory,i,j,colors[j+1+inc]);
              scoAddPolylineForLongDraw(pScopeMemory,i,j,colors[j+1+inc]);
            }
          else
            {
              scoAddPolylineForShortDraw(pScopeMemory,i,j,-1);
              scoAddPolylineForLongDraw(pScopeMemory,i,j,-1);
            }
        }
      inc = j+inc; //not +1 because of we have exited the loop
    }
}

void scoDelCoupleOfPolylines(ScopeMemory * pScopeMemory)
{
  int i,j;
  int c__1 = 1;
  scoGraphicalObject pLongDraw;
  scoGraphicalObject pShortDraw;
  int NbrPtsLong, NbrPtsShort;
  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
  if(scoGetPointerScopeWindow(pScopeMemory) != NULL)
    {
      for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
        {
          pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,0);
          NbrPtsLong = pPOLYLINE_FEATURE(pLongDraw)->n1;
          if(NbrPtsLong + scoGetShortDrawSize(pScopeMemory,i) > scoGetLongDrawSize(pScopeMemory,i))
            {
              //We realloc because maybe if we add the shortdraw the size would be bigger than the longdraw size
              for(j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
                {
                  pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
                  scoReallocLongDraw(pLongDraw, NbrPtsLong, scoGetShortDrawSize(pScopeMemory,i), 1000);
                }
              scoSetLongDrawSize(pScopeMemory,i,NbrPtsLong + scoGetShortDrawSize(pScopeMemory,i) + 1000);
            }
      
          pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,0);
          NbrPtsShort = pPOLYLINE_FEATURE(pShortDraw)->n1;
          pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,0);
          NbrPtsLong = pPOLYLINE_FEATURE(pLongDraw)->n1;
      
          for(j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
            {
              pShortDraw = scoGetPointerShortDraw(pScopeMemory,i,j);
              pLongDraw = scoGetPointerLongDraw(pScopeMemory,i,j);
              //Here we copy values in memory
              C2F(dcopy)(&NbrPtsShort,pPOLYLINE_FEATURE(pShortDraw)->pvx,&c__1,pPOLYLINE_FEATURE(pLongDraw)->pvx+NbrPtsLong,&c__1);
              C2F(dcopy)(&NbrPtsShort,pPOLYLINE_FEATURE(pShortDraw)->pvy,&c__1,pPOLYLINE_FEATURE(pLongDraw)->pvy+NbrPtsLong,&c__1);

              pPOLYLINE_FEATURE(pLongDraw)->n1 = NbrPtsLong + NbrPtsShort;
              pPOLYLINE_FEATURE(pShortDraw)->n1 = 0;
              //Destruction of the polyline - no presence in the menu editor anymore
              DestroyPolyline(pShortDraw);
	      //** TEST
              forceRedraw(pLongDraw);
            }
        }
       //** sciSetUsedWindow(scoGetWindowID(pScopeMemory));
      
      
      sciDrawObj(scoGetPointerScopeWindow(pScopeMemory));
      //**redrawHierarchy(scoGetPointerScopeWindow(pScopeMemory));
    }
}

void scoAddCoupleOfSegments(ScopeMemory * pScopeMemory, int * color)
{
  int i,j;
  int longdraw_size;
  double vx1[2], vy1[2];
  double * vx2, * vy2;
  int style[1];
  scoGraphicalObject pShortDraw;
  scoGraphicalObject pLongDraw;
  scoGraphicalObject pAxes;

  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
  for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      longdraw_size = scoGetLongDrawSize(pScopeMemory,i);
      //ConstructSegs dont support for the moment the NULL parameter
      vx1[0] = 0.0;
      vx1[1] = 0.0;
      vy1[0] = 0.0;
      vy1[1] = 0.0;
      
      vx2 = (double*)scicos_malloc(longdraw_size*sizeof(double));
      vy2 = (double*)scicos_malloc(longdraw_size*sizeof(double));

      for(j = 0 ; j < longdraw_size ; j++)
        {
          //same than before
          vx2[j] = 0.0;
          vy2[j] = 0.0;
        }
      
      
      

      pAxes = scoGetPointerAxes(pScopeMemory,i);

      for(j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
        {

          /* Compute color of segments */
          if (color == NULL)
          {
            /* default color */
            style[0] = 0;
          }
          else
          {
            /* User specified color */
            style[0] = color[j];
          }

          pLongDraw = ConstructSegs(pAxes, 0, vx2, vy2, NULL, longdraw_size, longdraw_size, 0, NULL, NULL,0,style, 0, TRUE, 0);
          pSEGS_FEATURE(pLongDraw)->Nbr1 = 0;
          pSEGS_FEATURE(pLongDraw)->Nbr2 = 0;
          sciSetIsLine(pLongDraw, 1);
          sciSetLineStyle(pLongDraw, 1);
          sciSetMarkStyle(pLongDraw, 0);
          sciSetIsMark(pLongDraw, 0);
          sciSetIsClipping(pLongDraw, 0);
          scoSetHandleFromPointerLongDraw(pScopeMemory,i,j,pLongDraw);

          pShortDraw = ConstructSegs(pAxes, 0, vx1, vy1, NULL, 2, 2, 0, NULL, NULL,0,style, 0, TRUE, 0);
          pSEGS_FEATURE(pShortDraw)->Nbr1 = 0;
          pSEGS_FEATURE(pShortDraw)->Nbr2 = 0;

          sciSetIsLine(pShortDraw, 1);
          sciSetLineStyle(pShortDraw, 1);
          sciSetMarkStyle(pShortDraw, 0);
          sciSetIsMark(pShortDraw, 0);
          sciSetIsClipping(pShortDraw, 0);
          scoSetHandleFromPointerShortDraw(pScopeMemory,i,j,pShortDraw);
        }

      scicos_free(vx2);
      scicos_free(vy2);
    }
}

void scoDelCoupleOfSegments(ScopeMemory * pScopeMemory)
{
	/* destroy all short draw */
	int j;
	int i;
	for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
  {
		for(j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
		{
			sciDelGraphicObj(scoGetPointerShortDraw(pScopeMemory, i, j));
		}
	}
	sciDrawObj(scoGetPointerScopeWindow(pScopeMemory));
}


scoGraphicalObject scoCreateSphere(scoGraphicalObject pAxes, double radius, int color)
{
  scoGraphicalObject pSphere;
  pSphere=ConstructArc(pAxes, 0, 0, radius, radius, 0, 23040, &color, &color, TRUE, FALSE);
  return pSphere;
}

void scoAddSphereForShortDraw(ScopeMemory * pScopeMemory, int i, int j, double radius, int color)
{
  scoGraphicalObject pShortDraw;
  pShortDraw = scoCreateSphere(scoGetPointerAxes(pScopeMemory,i),radius,color);
  scoSetHandleFromPointerShortDraw(pScopeMemory,i,j,pShortDraw);
}

void scoAddSphereForLongDraw(ScopeMemory * pScopeMemory, int i, int j, double radius, int color)
{
  scoGraphicalObject pLongDraw;
  pLongDraw = scoCreateSphere(scoGetPointerAxes(pScopeMemory,i),radius,color);
  scoSetHandleFromPointerLongDraw(pScopeMemory,i,j,pLongDraw);
}

void scoAddCoupleOfSpheres(ScopeMemory * pScopeMemory, double * radius, int * colors)
{
  int i,j;
  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
  for (i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    { 
      for(j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,i) ; j++)
        {
          if(colors != NULL)
            {
              scoAddSphereForShortDraw(pScopeMemory,i,j,radius[i+j], colors[i+j]);
              scoAddSphereForLongDraw(pScopeMemory,i,j,radius[i+j], colors[i+j]);
            }
          else
            {
              scoAddSphereForShortDraw(pScopeMemory,i,j,1,1);
              scoAddSphereForLongDraw(pScopeMemory,i,j,1,1);
            }
        }
    }
}

scoGraphicalObject scoCreateRectangle(scoGraphicalObject pAxes, double x, double y, double width, double height)
{
  scoGraphicalObject pRectangle;
  pRectangle = ConstructRectangle(pAxes,x,y,height,width,NULL,NULL,0,1);
  return pRectangle;
}

void scoAddRectangleForLongDraw(ScopeMemory * pScopeMemory, int i, int j, double x, double y, double width, double height)
{
  scoGraphicalObject pLongDraw;
  pLongDraw = scoCreateRectangle(scoGetPointerAxes(pScopeMemory,i),x,y,width,height);
  scoSetHandleFromPointerLongDraw(pScopeMemory,i,j,pLongDraw);
}

void scoAddTitlesScope(ScopeMemory * pScopeMemory, char * x, char * y, char * z)
{
  extern int get_block_number();
  int i;
  int nxname;
  int kfun;
  char buf[40];
  int i__1;
  char ** title=NULL;
  char * x_title=NULL;
  char * y_title;

  title = (char**)scicos_malloc(scoGetNumberOfSubwin(pScopeMemory)*sizeof(char*));
  for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      title[i] = (char*)scicos_malloc(20*sizeof(char)); //Why 20 ? BECAUSE :) should be less than 255
      sprintf(title[i],"Graphic %d",i+1);
    }

  
  x_title = x;
  y_title = y;

  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
  for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
                sciSetText(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_title,&title[i],1,1); /* 1,1 is nbrow, nbcol */
      sciSetText(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_x_label,&x_title,1,1); /* 1,1 is nbrow, nbcol */
      sciSetText(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_y_label,&y_title,1,1);/* 1,1 is nbrow, nbcol */

      
      sciSetFontSize(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_x_label, 1);
      sciSetFontSize(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_y_label, 1);
      sciSetFontSize(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_title, 1);
      forceRedraw(scoGetPointerAxes(pScopeMemory,i));
    }

  if(z!= NULL)
    {
      char * z_title;
      z_title = z;

      for(i = 0 ; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
        {
          sciSetFontSize(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_z_label, 1);
          sciSetText(pSUBWIN_FEATURE(scoGetPointerAxes(pScopeMemory,i))->mon_z_label,&z_title,1,1); /* 1,1 is nbrow, nbcol */
        }
    }
  for(i = 0; i < scoGetNumberOfSubwin(pScopeMemory) ; i++)
    {
      scicos_free(title[i]);
    }
  scicos_free(title);

  /* Code for naming the window*/
  nxname = 40;
  kfun = get_block_number();
  C2F(getlabel)(&kfun, buf, &nxname);
  if(nxname > 39)
    {
      nxname = 39;
    }
  i__1 = nxname;
  *(buf+i__1) = *"\000";
  if((nxname == 1 && *(unsigned char *)buf == ' ') || (nxname ==0))
    {
      /*do nothing*/
    }
  else
    {
      sciSetName(scoGetPointerScopeWindow(pScopeMemory), buf);
    }
  /*End of code for naming window */
  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
   sciDrawObj(scoGetPointerScopeWindow(pScopeMemory));
  //**redrawHierarchy(scoGetPointerScopeWindow(pScopeMemory));
}

void scoDrawScopeXYStyle(ScopeMemory * pScopeMemory)
{
  scoGraphicalObject Pinceau; //Pencil
  scoGraphicalObject Trait; //Line
  int NbrPtsShort, NbrPtsLong;
  int c__1 = 1;
  int i,j;
  //Coded only for one subwin can be easily extended to many with a factorj , j < scoGetNumberOfSubwin()
  for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ;i++)
    {
      Pinceau = scoGetPointerShortDraw(pScopeMemory,0,i);
      Trait = scoGetPointerLongDraw(pScopeMemory,0,i);

      NbrPtsShort = pPOLYLINE_FEATURE(Pinceau)->n1;
      if(NbrPtsShort >= scoGetShortDrawSize(pScopeMemory,0))
        {
          sciSetUsedWindow(scoGetWindowID(pScopeMemory));
          //pixmap
          if(sciGetPixmapMode(scoGetPointerScopeWindow(pScopeMemory)))
            {
                        /* TODO : not implemented */
                        /*C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);*/
            }
          sciDrawSingleObj(Pinceau);
          NbrPtsLong = pPOLYLINE_FEATURE(Trait)->n1;
          if(NbrPtsLong + NbrPtsShort >= scoGetLongDrawSize(pScopeMemory,0))
            {
              for(j = 0 ; j < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ; j++)
                {
                  scoReallocLongDraw(scoGetPointerLongDraw(pScopeMemory,0,j), NbrPtsLong, NbrPtsShort, 5000);
                }
              scoSetLongDrawSize(pScopeMemory, 0, NbrPtsLong + NbrPtsShort + 5000);
            }
          NbrPtsLong = pPOLYLINE_FEATURE(Trait)->n1;
          C2F(dcopy)(&NbrPtsShort,pPOLYLINE_FEATURE(Pinceau)->pvx,&c__1,pPOLYLINE_FEATURE(Trait)->pvx+NbrPtsLong,&c__1);
          C2F(dcopy)(&NbrPtsShort,pPOLYLINE_FEATURE(Pinceau)->pvy,&c__1,pPOLYLINE_FEATURE(Trait)->pvy+NbrPtsLong,&c__1);
          if(pPOLYLINE_FEATURE(Pinceau)->pvz != NULL)
            {
              C2F(dcopy)(&NbrPtsShort,pPOLYLINE_FEATURE(Pinceau)->pvz,&c__1,pPOLYLINE_FEATURE(Trait)->pvz+NbrPtsLong,&c__1);
            }      
          pPOLYLINE_FEATURE(Trait)->n1 = NbrPtsLong + NbrPtsShort;
          pPOLYLINE_FEATURE(Pinceau)->pvx[0] = pPOLYLINE_FEATURE(Pinceau)->pvx[NbrPtsShort-1];
          pPOLYLINE_FEATURE(Pinceau)->pvy[0] = pPOLYLINE_FEATURE(Pinceau)->pvy[NbrPtsShort-1];
          if(pPOLYLINE_FEATURE(Pinceau)->pvz != NULL)
            {
              pPOLYLINE_FEATURE(Pinceau)->pvz[0] = pPOLYLINE_FEATURE(Pinceau)->pvz[NbrPtsShort-1];
            }
          pPOLYLINE_FEATURE(Pinceau)->n1 = 1;
        }
    }
}

void scoDrawScopeAnimXYStyle(ScopeMemory * pScopeMemory, double * u1, double * u2, double * u3)
{
  int i,j;
  scoGraphicalObject Gomme; //Rubber
  scoGraphicalObject Pinceau; //Pencil
  scoGraphicalObject Trait; //Line
  int nbr_curves;


  /*If only one element to draw*/
  if(scoGetLongDrawSize(pScopeMemory,0) == 0)
    {
      for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0) ; i++)
        {
          Pinceau = scoGetPointerShortDraw(pScopeMemory,0,i);
          pPOLYLINE_FEATURE(Pinceau)->pvx[0] = u1[i];
          pPOLYLINE_FEATURE(Pinceau)->pvy[0] = u2[i];
          if(u3 != NULL)
            {
              pPOLYLINE_FEATURE(Pinceau)->pvz[0] = u3[i];
            }
        }

      sciSetUsedWindow(scoGetWindowID(pScopeMemory));
      if(sciGetPixmapMode(scoGetPointerScopeWindow(pScopeMemory)))
        {
                /* TODO : not implemented */
                /*C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);*/
        }
      //** redrawHierarchy(scoGetPointerScopeWindow(pScopeMemory));
      sciDrawObj(scoGetPointerScopeWindow(pScopeMemory));
    }
  //if a lot of elements to draw (more than 2 :p)
  else
    {
      nbr_curves = scoGetNumberOfCurvesBySubwin(pScopeMemory,0)/2;
      /*if it is a mark style scope*/
      if(scoGetShortDrawSize(pScopeMemory,0) == 1)
        {
          //3D scope Mode
          if(u3 != NULL)
            {
              for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0)/2 ; i++)
                {
                  Pinceau = scoGetPointerShortDraw(pScopeMemory,0,i);
                  Gomme = scoGetPointerShortDraw(pScopeMemory,0,i+nbr_curves);
                  Trait = scoGetPointerLongDraw(pScopeMemory,0,i);
                
                  pPOLYLINE_FEATURE(Gomme)->pvx[0] = pPOLYLINE_FEATURE(Trait)->pvx[scoGetLongDrawSize(pScopeMemory,0)-1];
                  pPOLYLINE_FEATURE(Gomme)->pvy[0] = pPOLYLINE_FEATURE(Trait)->pvy[scoGetLongDrawSize(pScopeMemory,0)-1];
                  pPOLYLINE_FEATURE(Gomme)->pvz[0] = pPOLYLINE_FEATURE(Trait)->pvz[scoGetLongDrawSize(pScopeMemory,0)-1];

                  for (j = scoGetLongDrawSize(pScopeMemory,0)-1 ; j > 0 ; j--)
                    {
                      pPOLYLINE_FEATURE(Trait)->pvx[j] = pPOLYLINE_FEATURE(Trait)->pvx[j-1];
                      pPOLYLINE_FEATURE(Trait)->pvy[j] = pPOLYLINE_FEATURE(Trait)->pvy[j-1];
                      pPOLYLINE_FEATURE(Trait)->pvz[j] = pPOLYLINE_FEATURE(Trait)->pvz[j-1];
                    }

                  pPOLYLINE_FEATURE(Trait)->pvx[0] = pPOLYLINE_FEATURE(Pinceau)->pvx[0];
                  pPOLYLINE_FEATURE(Trait)->pvy[0] = pPOLYLINE_FEATURE(Pinceau)->pvy[0];
                  pPOLYLINE_FEATURE(Trait)->pvz[0] = pPOLYLINE_FEATURE(Pinceau)->pvz[0];
                
                  pPOLYLINE_FEATURE(Pinceau)->pvx[0] = u1[i];
                  pPOLYLINE_FEATURE(Pinceau)->pvy[0] = u2[i];
                  pPOLYLINE_FEATURE(Pinceau)->pvz[0] = u3[i];           

                  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
                  if(sciGetPixmapMode(scoGetPointerScopeWindow(pScopeMemory)))
                    {
                                /* TODO : not implemented */
                                /*C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);*/
                    }
                  sciDrawSingleObj(Pinceau);
                  sciDrawSingleObj(Gomme);
                }
            }
          //2D Scope Mode
          else
            {
              for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0)/2 ; i++)
                {
                  Pinceau = scoGetPointerShortDraw(pScopeMemory,0,i);
                  Gomme = scoGetPointerShortDraw(pScopeMemory,0,i+nbr_curves);
                  Trait = scoGetPointerLongDraw(pScopeMemory,0,i);
                
                  pPOLYLINE_FEATURE(Gomme)->pvx[0] = pPOLYLINE_FEATURE(Trait)->pvx[scoGetLongDrawSize(pScopeMemory,0)-1];
                  pPOLYLINE_FEATURE(Gomme)->pvy[0] = pPOLYLINE_FEATURE(Trait)->pvy[scoGetLongDrawSize(pScopeMemory,0)-1];

                  for (j = scoGetLongDrawSize(pScopeMemory,0)-1 ; j > 0 ; j--)
                    {
                      pPOLYLINE_FEATURE(Trait)->pvx[j] = pPOLYLINE_FEATURE(Trait)->pvx[j-1];
                      pPOLYLINE_FEATURE(Trait)->pvy[j] = pPOLYLINE_FEATURE(Trait)->pvy[j-1];
                    }

                  pPOLYLINE_FEATURE(Trait)->pvx[0] = pPOLYLINE_FEATURE(Pinceau)->pvx[0];
                  pPOLYLINE_FEATURE(Trait)->pvy[0] = pPOLYLINE_FEATURE(Pinceau)->pvy[0];
                
                  pPOLYLINE_FEATURE(Pinceau)->pvx[0] = u1[i];
                  pPOLYLINE_FEATURE(Pinceau)->pvy[0] = u2[i];

                  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
                  if(sciGetPixmapMode(scoGetPointerScopeWindow(pScopeMemory)))
                    {
                                /* TODO : not implemented */
                                /*C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);*/
                    }
                  sciDrawSingleObj(Pinceau);
                  sciDrawSingleObj(Gomme);
                }
            }
        }
      /*if it is a line style scope*/
      else
        {
          //3D Scope Mode
          if(u3 != NULL)
            {
              for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0)/2 ; i++)
                {
                  Pinceau = scoGetPointerShortDraw(pScopeMemory,0,i);
                  Gomme = scoGetPointerShortDraw(pScopeMemory,0,i+nbr_curves);
                  Trait = scoGetPointerLongDraw(pScopeMemory,0,i);

                  pPOLYLINE_FEATURE(Gomme)->pvx[0] = pPOLYLINE_FEATURE(Trait)->pvx[scoGetLongDrawSize(pScopeMemory,0)-1];
                  pPOLYLINE_FEATURE(Gomme)->pvy[0] = pPOLYLINE_FEATURE(Trait)->pvy[scoGetLongDrawSize(pScopeMemory,0)-1];
                  pPOLYLINE_FEATURE(Gomme)->pvz[0] = pPOLYLINE_FEATURE(Trait)->pvz[scoGetLongDrawSize(pScopeMemory,0)-1];

                  pPOLYLINE_FEATURE(Gomme)->pvx[1] = pPOLYLINE_FEATURE(Trait)->pvx[scoGetLongDrawSize(pScopeMemory,0)-2];
                  pPOLYLINE_FEATURE(Gomme)->pvy[1] = pPOLYLINE_FEATURE(Trait)->pvy[scoGetLongDrawSize(pScopeMemory,0)-2];
                  pPOLYLINE_FEATURE(Gomme)->pvz[1] = pPOLYLINE_FEATURE(Trait)->pvz[scoGetLongDrawSize(pScopeMemory,0)-2];

                  for (j = scoGetLongDrawSize(pScopeMemory,0)-1 ; j > 0 ; j--)
                    {
                      pPOLYLINE_FEATURE(Trait)->pvx[j] = pPOLYLINE_FEATURE(Trait)->pvx[j-1];
                      pPOLYLINE_FEATURE(Trait)->pvy[j] = pPOLYLINE_FEATURE(Trait)->pvy[j-1];
                      pPOLYLINE_FEATURE(Trait)->pvz[j] = pPOLYLINE_FEATURE(Trait)->pvz[j-1];
                    }

                  pPOLYLINE_FEATURE(Trait)->pvx[0] = pPOLYLINE_FEATURE(Pinceau)->pvx[0];
                  pPOLYLINE_FEATURE(Trait)->pvy[0] = pPOLYLINE_FEATURE(Pinceau)->pvy[0];
                  pPOLYLINE_FEATURE(Trait)->pvz[0] = pPOLYLINE_FEATURE(Pinceau)->pvz[0];
                
                  pPOLYLINE_FEATURE(Pinceau)->pvx[1] = pPOLYLINE_FEATURE(Pinceau)->pvx[0];
                  pPOLYLINE_FEATURE(Pinceau)->pvy[1] = pPOLYLINE_FEATURE(Pinceau)->pvy[0];
                  pPOLYLINE_FEATURE(Pinceau)->pvz[1] = pPOLYLINE_FEATURE(Pinceau)->pvz[0];

                  pPOLYLINE_FEATURE(Pinceau)->pvx[0] = u1[i];
                  pPOLYLINE_FEATURE(Pinceau)->pvy[0] = u2[i];
                  pPOLYLINE_FEATURE(Pinceau)->pvz[0] = u3[i];

                  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
                  if(sciGetPixmapMode(scoGetPointerScopeWindow(pScopeMemory)))
                    {
                                /* TODO : not implemented */
                                /*C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);*/
                    }
                  sciDrawSingleObj(Pinceau);
                  sciDrawSingleObj(Gomme);
                }
            }
          //2D Scope Mode
          else
            {
              for(i = 0 ; i < scoGetNumberOfCurvesBySubwin(pScopeMemory,0)/2 ; i++)
                {
                  Pinceau = scoGetPointerShortDraw(pScopeMemory,0,i);
                  Gomme = scoGetPointerShortDraw(pScopeMemory,0,i+nbr_curves);
                  Trait = scoGetPointerLongDraw(pScopeMemory,0,i);

                  pPOLYLINE_FEATURE(Gomme)->pvx[0] = pPOLYLINE_FEATURE(Trait)->pvx[scoGetLongDrawSize(pScopeMemory,0)-1];
                  pPOLYLINE_FEATURE(Gomme)->pvy[0] = pPOLYLINE_FEATURE(Trait)->pvy[scoGetLongDrawSize(pScopeMemory,0)-1];

                  pPOLYLINE_FEATURE(Gomme)->pvx[1] = pPOLYLINE_FEATURE(Trait)->pvx[scoGetLongDrawSize(pScopeMemory,0)-2];
                  pPOLYLINE_FEATURE(Gomme)->pvy[1] = pPOLYLINE_FEATURE(Trait)->pvy[scoGetLongDrawSize(pScopeMemory,0)-2];

                  for (j = scoGetLongDrawSize(pScopeMemory,0)-1 ; j > 0 ; j--)
                    {
                      pPOLYLINE_FEATURE(Trait)->pvx[j] = pPOLYLINE_FEATURE(Trait)->pvx[j-1];
                      pPOLYLINE_FEATURE(Trait)->pvy[j] = pPOLYLINE_FEATURE(Trait)->pvy[j-1];
                    }

                  pPOLYLINE_FEATURE(Trait)->pvx[0] = pPOLYLINE_FEATURE(Pinceau)->pvx[0];
                  pPOLYLINE_FEATURE(Trait)->pvy[0] = pPOLYLINE_FEATURE(Pinceau)->pvy[0];
                
                  pPOLYLINE_FEATURE(Pinceau)->pvx[1] = pPOLYLINE_FEATURE(Pinceau)->pvx[0];
                  pPOLYLINE_FEATURE(Pinceau)->pvy[1] = pPOLYLINE_FEATURE(Pinceau)->pvy[0];

                  pPOLYLINE_FEATURE(Pinceau)->pvx[0] = u1[i];
                  pPOLYLINE_FEATURE(Pinceau)->pvy[0] = u2[i];

                  sciSetUsedWindow(scoGetWindowID(pScopeMemory));
                  if(sciGetPixmapMode(scoGetPointerScopeWindow(pScopeMemory)))
                    {
                                /* TODO : not implemented */
                                /*C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);*/
                    }
                  sciDrawSingleObj(Pinceau);
                  sciDrawSingleObj(Gomme);
                }
            }
        }
         
    }
}

scoGraphicalObject scoCreateGrayplot(scoGraphicalObject pAxes, int size_x, int size_y)
{
  scoGraphicalObject pGrayplot;
  double * pvecx, * pvecy, * pvecz;
  int i;

  pvecx = (double*)scicos_malloc(size_x*sizeof(double));
  for(i = 0; i < size_x ; i++)
    pvecx[i] = i;

  pvecy = (double*)scicos_malloc(size_y*sizeof(double));
  for(i = 0; i < size_y ; i++)
    pvecy[i] = i;

  pvecz = (double*)scicos_malloc(size_x*size_y*sizeof(double));
  for(i = 0; i < size_x*size_y ; i++)
    pvecz[i] = 0;
  
  pGrayplot = ConstructGrayplot(pAxes, pvecx, pvecy, pvecz, size_x, size_y, 0);

  scicos_free(pvecx);
  scicos_free(pvecy);
  scicos_free(pvecz);

  return pGrayplot;
}

void scoAddGrayplotForShortDraw(ScopeMemory * pScopeMemory, int i, int j, int size_x, int size_y)
{
  scoGraphicalObject pShortDraw;
  pShortDraw = scoCreateGrayplot(scoGetPointerAxes(pScopeMemory,i),size_x,size_y);
  scoSetHandleFromPointerShortDraw(pScopeMemory,i,j,pShortDraw);
}

scoGraphicalObject scoCreatePlot3d(scoGraphicalObject pAxes, int size_x, int size_y)
{
  scoGraphicalObject pPlot3d;
  double * pvecx, * pvecy, *pvecz;
  int i;
  int flag[3];
  double ebox[6];
  int isfac;
  int m1,n1,m2,n2,m3,n3,m3n,n3n;
  int colorflag ;

  pvecx = (double*)scicos_malloc(size_x*sizeof(double));
  for(i = 0; i < size_x ; i++)
    pvecx[i] = i;

  pvecy = (double*)scicos_malloc(size_y*sizeof(double));
  for(i = 0; i < size_y ; i++)
    pvecy[i] = i;

  pvecz = (double*)scicos_malloc(size_x*size_y*sizeof(double));
  for(i = 0; i < size_x*size_y ; i++)
    pvecz[i] = 0;

  //I have found all these values thanks to gdb and debugging a demo of scilab with a grayplot. I don't know exactly what they are and I don't care :p
  flag[0] = 2;
  flag[1] = 8;
  flag[2] = 4;

  ebox[0] = 0;
  ebox[1] = 1;
  ebox[2] = 0;
  ebox[3] = 1;
  ebox[4] = 0;
  ebox[5] = 1;

  isfac = 0;

  colorflag = 1;

  m1 = 1;
  n1 = size_x;
  m2 = 1;
  n2 = size_y;
  m3 =  size_x;
  n3 = size_y;
  m3n = 0;
  n3n = 0;
  
  pPlot3d = ConstructSurface (pAxes, SCI_PLOT3D, pvecx, pvecy, pvecz, NULL, 0, size_x, size_y, flag, ebox, colorflag, &isfac, &m1, &n1, &m2, &n2, &m3, &n3, &m3n, &n3n);

  scicos_free(pvecx);
  scicos_free(pvecy);
  scicos_free(pvecz);

  return pPlot3d;
}

void scoAddPlot3dForShortDraw(ScopeMemory * pScopeMemory, int i, int j, int size_x, int size_y)
{
  scoGraphicalObject pShortDraw;
  pShortDraw = scoCreatePlot3d(scoGetPointerAxes(pScopeMemory,i),size_x,size_y);
  scoSetHandleFromPointerShortDraw(pScopeMemory,i,j,pShortDraw);
}
