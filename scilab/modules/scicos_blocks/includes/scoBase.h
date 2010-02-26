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
\file scoBase.h
\author Benoit Bayol
\version 1.0
\date September 2006 - January 2007
\brief Base file of the SCO Library
*/


#ifndef __SCO_BASE_H__
#define __SCO_BASE_H__

/*----------------------------------INCLUDES-----------------------*/

#include "machine.h"

#include "dynlib_scicos_blocks.h"

/*Graphic Library*/
#include "SetProperty.h"
#include "GetProperty.h"
#include "InitObjects.h"
#include "DrawObjects.h"
#include "BuildObjects.h"
#include "ObjectStructure.h"
#include "DestroyObjects.h"
#include "ObjectStructure.h"

#include "MALLOC.h"

/*--------------------------------TYPEDEF---------------------*/

/**
\brief scoGraphicalObject is an equivalent of sciPointObj * 
*/
typedef sciPointObj * scoGraphicalObject;

/**
\brief scoLineOfGraphicalObjects is a table of scoGraphicalObject
\attention No plural here for GraphicalObject 
*/
typedef scoGraphicalObject * scoLineOfGraphicalObject;

/**
\brief scoMatrixOfGraphicalObjects is a table of scoLineOfGraphicalObjects or a matrix of scoGraphicalObject
\attention No plural here for GraphicalObject
*/
typedef scoLineOfGraphicalObject * scoMatrixOfGraphicalObject;

/**
\brief scoInteger is an equivalent of int 
*/
typedef int scoInteger;

/**
\brief scoLineOfInteger is a table of scoInteger 
*/
typedef scoInteger * scoLineOfInteger;

/**
\brief scoLongInteger is an equivalent of long
*/
typedef long scoLongInteger;

/**
\brief scoLineOfLongInteger is a table of scoLongInteger
*/
typedef scoLongInteger * scoLineOfLongInteger;

/**
\brief scoMatrixOfLongInteger is a table of scoLineOfLongInteger or a matrix of scoLongInteger 
*/
typedef scoLineOfLongInteger * scoMatrixOfLongInteger;

/**
\brief scoDouble is an equivalent of double 
*/
typedef double scoDouble;

/**
\brief scoLineOfDouble is a table of scoDouble 
*/
typedef scoDouble * scoLineOfDouble;

/** \brief ScopeMemory is useful to stock in the block->work the whole information we need to proceed the draw of the scope. Attention we are using the block->work because we cannot use a static structure (without a huge modification of the behavior) because there will be problems if we have two blocks of the same nature in the diagram
\param number_of_subwin Number of Subwin (Axes) in the whole Window
\param win_id Windows ID
\param shortdraw_size Size of the Buffer of an axes i i.e. shortdraw_size[i]
\param new_draw Determines if we have to do a new draw of an axes i i.e. new_draw[i]
\param period_counter When we are using a timed scope it allows to calculate the X scale
\param longdraw_size When we have a non direct scope we have to know the length of the trace to be stocked
\param number_of_curves_by_subwin The number of curve by subwin (axes) i i.e. number_of_curves_by_subwin[i]
\param hScopeWindow the Handle on the Scope Window
\param hAxes the Handles of an axes i i.e. hAxes[i]
\param hShortDraw the Handle of a ShortDraw j on an axes i i.e. hShortDraw[i][j]
\param hLongDraw the Handle of a LongDraw j on an axes i i.e. hLongDraw[i][j]
\param period the period of the axes i i.e. period[i]
*/
typedef struct
{
	scoInteger number_of_subwin;
	scoInteger win_id;

	scoLineOfInteger   shortdraw_size;
	scoLineOfInteger   new_draw;
	scoLineOfInteger   period_counter;
	scoLineOfInteger   longdraw_size;
	scoLineOfInteger   number_of_curves_by_subwin;

	scoLongInteger hScopeWindow;

	scoLineOfLongInteger   hAxes;

	scoMatrixOfLongInteger hShortDraw;
	scoMatrixOfLongInteger hLongDraw;

	scoLineOfDouble   period;

	scoInteger activated;

	double     d_last_scope_update_time ; 

} ScopeMemory;

/*External function of malloc/free - Don't forget to use these and not malloc or MALLOC*/

#endif

