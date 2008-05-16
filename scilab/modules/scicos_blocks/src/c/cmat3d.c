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
   \file cmat3d.c
   \author Benoit Bayol
   \version 1.0
   \date September 2006 - January 2007
   \brief CMAT3D is a scope which connect a matrix to a plot3d. Values of the matrix are the values at the nodes.
   \see CMAT3D.sci in macros/scicos_blocks/Sinks/
*/
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"
#include "scoMemoryScope.h"
#include "scoWindowScope.h"
#include "scoMisc.h"
#include "scoGetProperty.h"
#include "scoSetProperty.h"
#include "scicos_block4.h"

/** \fn cmat3d_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
    \brief Function to draw or redraw the window
*/
void cmat3d_draw(scicos_block * block, ScopeMemory ** pScopeMemory, int firstdraw)
{
  /*Declarations*/
  int i; //As usual
  int * ipar; //Integer Parameters
  int win_pos[2]; //Position of the Window
  int win_dim[2]; //Dimension of the Window
  int dimension = 3;
  double * rpar; //Reals parameters
  double  ymin, ymax; //Ymin and Ymax are vectors here
  double  xmin, xmax;
  double zmin, zmax;
  int number_of_curves_by_subwin;
  int number_of_subwin;
  double * mat;
  int size_mat;
  int size_in_x;
  int size_in_y;
  scoGraphicalObject pShortDraw;

  /*Retrieve parameters from the scicos_model() which has been created thanks to the interfacing function*/
  rpar = GetRparPtrs(block);
  ipar = GetIparPtrs(block);

  number_of_subwin = 1;

  win_pos[0] = -1;
  win_pos[1] = -1;
  win_dim[0] = -1;
  win_dim[1] = -1;

  size_mat = ipar[2];
  mat = (double*)scicos_malloc(size_mat*sizeof(double));
  for(i = 0 ; i < size_mat ; i++)
    {
      mat[i] = rpar[i];
    }
  size_in_x = GetInPortSize(block,1,1);
  size_in_y = GetInPortSize(block,1,2);
  if(ipar[3] == 1)
    {
      xmax = size_in_x;
      xmin = 0;
      ymax = size_in_y;
      ymin = 0;
    }
  else
    {
      xmin = rpar[size_mat];
      xmax = rpar[size_mat+1];
      ymin = rpar[size_mat+2];
      ymax = rpar[size_mat+3];
    }

  zmin = ipar[0];
  zmax = ipar[1];
  number_of_curves_by_subwin = 1;

  /*Allocating memory for scope only if the window has to be created and not redraw*/
  if(firstdraw == 1)
    {
      scoInitScopeMemory(block->work,pScopeMemory, number_of_subwin, &number_of_curves_by_subwin);
    }

  /*Creating the Scope with axes*/
  scoInitOfWindow(*pScopeMemory, dimension, -1, win_pos, win_dim, &xmin, &xmax, &ymin, &ymax, &zmin, &zmax);
  if(scoGetScopeActivation(*pScopeMemory) == 1)
    {
      /*Here we put the special window feature like pixmap or text title
	Dont forget that the function scoAddTitleScope redraws the window at end so it would be a good idea to put it at the end*/
      //sciSetPixmapMode(scoGetPointerScopeWindow(*pScopeMemory),TRUE);
      //pFIGURE_FEATURE(scoGetPointerScopeWindow(*pScopeMemory))->pixmapMode = 1;

      sciSetColormap(scoGetPointerScopeWindow(*pScopeMemory), mat , size_mat/3, 3);

      pSUBWIN_FEATURE(scoGetPointerAxes(*pScopeMemory,0))->alpha = 50;
      pSUBWIN_FEATURE(scoGetPointerAxes(*pScopeMemory,0))->theta = 280;
  
      /*Adding graphic elements like plot3d or polyline and so*/
      if(ipar[3] == 1)
	{
	  scoAddPlot3dForShortDraw(*pScopeMemory,0,0,GetInPortSize(block,1,1),GetInPortSize(block,1,2));
	}
      else
	{
	  double h_x,h_y;
	  scoAddPlot3dForShortDraw(*pScopeMemory,0,0,GetInPortSize(block,1,1),GetInPortSize(block,1,2));
	  pShortDraw = scoGetPointerShortDraw(*pScopeMemory,0,0);
	  h_x = fabs((xmax-xmin)/(GetInPortSize(block,1,1)-1));
	  h_y = fabs((ymax-ymin)/(GetInPortSize(block,1,2)-1));
      
	  for(i = 0 ; i < size_in_x ; i++)
	    {
	      pSURFACE_FEATURE(pShortDraw)->pvecx[i] = xmin + i*h_x;
	    } 
	  for(i = 0 ; i < size_in_y ; i++)
	    {
	      pSURFACE_FEATURE(pShortDraw)->pvecy[i] = ymin + i*h_y;
	    } 
	}
      scoAddTitlesScope(*pScopeMemory,"x","y","z");
    }
  /*Dont forget to free your scicos_malloc or MALLOC*/
  scicos_free(mat);

}

/** \fn void cmat3d(scicos_block * block, int flag)
    \brief the computational function
    \param block A pointer to a scicos_block
    \param flag An integer which indicates the state of the block (init, update, ending)
*/
void cmat3d(scicos_block * block, int flag)
{
  /* Declarations */
  ScopeMemory * pScopeMemory;
  scoGraphicalObject pShortDraw;
  double * u1;
  int i,j;
  int dim_i, dim_j;
 
  /* State Machine Control */
  switch(flag)
    {
      /*Flag 4*/
    case Initialization:
      {
        /*We create the window for the first time, so 1 is in parameters*/
	cmat3d_draw(block,&pScopeMemory,1);
	break; //dont forget the break
      }
      /*Flag 2*/
    case StateUpdate:
      {
	/*Retreiving Scope in the block->work*/
	scoRetrieveScopeMemory(block->work,&pScopeMemory);
	if(scoGetScopeActivation(pScopeMemory) == 1)
	  {

	    /* If window has been destroyed we recreate it */
	    if(scoGetPointerScopeWindow(pScopeMemory) == NULL)
	      {
		//0 here because of the recreation
		cmat3d_draw(block,&pScopeMemory,0);
	      }
	    /*Here some allocations and calcul wich are necessary*/
	    pShortDraw = scoGetPointerShortDraw(pScopeMemory,0,0);

	    u1 = GetInPortPtrs(block,1);
	    dim_i = GetInPortRows(block,1);
	    dim_j = GetInPortCols(block,1);

	    for(i = 0 ; i < dim_i ; i++)
	      {
	    
		for(j = 0; j < dim_j ; j++)
		  {
		    pSURFACE_FEATURE(pShortDraw)->pvecz[j+i*dim_j] = u1[j+dim_j*i];
		  }
	      }
        
	    /*Here is the draw instructions*/
	    sciSetUsedWindow(scoGetWindowID(pScopeMemory));
	    if(sciGetPixmapMode(scoGetPointerScopeWindow(pScopeMemory)))
	      {
			  /* TODO : not implemented */
			  /*C2F(dr)("xset","wshow",PI0,PI0,PI0,PI0,PI0,PI0,PD0,PD0,PD0,PD0,0L,0L);*/
	      }
	    forceRedraw(pShortDraw);
	    sciDrawObj(scoGetPointerScopeWindow(pScopeMemory));
	  }
	break; //dont forget the break
      }
      /*Flag 5*/
    case Ending:
      {
        /*Retrieve Memory*/
	scoRetrieveScopeMemory(block->work, &pScopeMemory);
        /*Here we can add specific instructions to be sure that we have stick short and longdraw if we need it. Cscope for example stick the last short to the long to have one curve to move*/
        /*Free Memory*/
	if(scoGetScopeActivation(pScopeMemory) == 1)
	  {
	    sciSetUsedWindow(scoGetWindowID(pScopeMemory));
	    pShortDraw = sciGetCurrentFigure();
	    pFIGURE_FEATURE(pShortDraw)->user_data = NULL;
	    pFIGURE_FEATURE(pShortDraw)->size_of_user_data = 0;
	  }
	scoFreeScopeMemory(block->work, &pScopeMemory);
	break;
      }
    }
}
