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
\file scoGetProperty.h
\author Benoit Bayol
\version 1.0
\date September 2006 - January 2007
\brief Header File of the scoGetProperty.c file 
*/

#ifndef __SCO_GET_PROPERTY_H__
#define __SCO_GET_PROPERTY_H__

#include "dynlib_scicos_blocks.h"
#include "scoBase.h"

/**
\brief Get the internal time in seconds (micro or nano second max resolution)
\return double
*/
SCICOS_BLOCKS_IMPEXP double scoGetRealTime(void);


/**
\brief Get the Handle of the Scope Window in the ScopeMemory
\param pScopeMemory a pointer on a ScopeMemory
\return hScopeWindow
*/
SCICOS_BLOCKS_IMPEXP scoLongInteger scoGetHandleScopeWindow(ScopeMemory * pScopeMemory);

/**
\brief Get the Handle of Axes i in the ScopeMemory where i is the index of the axes
\param pScopeMemory a pointer on a ScopeMemory
\param i index of an Axes
\return hAxes[i]
*/
SCICOS_BLOCKS_IMPEXP scoLongInteger scoGetHandleAxes(ScopeMemory * pScopeMemory, int i);

/**
\brief Get the Handle of the ShortDraw i,j in the ScopeMemory where i is the index of the axes and j is the index of the curve in the axes
\param pScopeMemory a pointer on a ScopeMemory
\param i index of an Axes
\param j index of a curve
\return hShortDraw[i][j]
*/
SCICOS_BLOCKS_IMPEXP scoLongInteger scoGetHandleShortDraw(ScopeMemory * pScopeMemory, int i, int j);

/**
\brief Get the Handle of the LongDraw i,j in the ScopeMemory where i is the index of the axes and j is the index of the curve in the axes
\param pScopeMemory a pointer on a ScopeMemory
\param i index of an Axes
\param j index of a curve
\return hLongDraw[i][j]
*/
SCICOS_BLOCKS_IMPEXP scoLongInteger scoGetHandleLongDraw(ScopeMemory * pScopeMemory, int i, int j);

/**
\brief Get the NewDraw value of Axes i in the ScopeMemory
\param pScopeMemory a pointer on a ScopeMemory
\param i index of an axes
\return new_draw[i]
*/
SCICOS_BLOCKS_IMPEXP scoInteger scoGetNewDraw(ScopeMemory * pScopeMemory, int i);

/**
\brief Get the PeriodCounter of Axes i in the ScopeMemory
\param pScopeMemory a pointer on a ScopeMemory
\param i index of an axes
\return period_counter[i]
*/
SCICOS_BLOCKS_IMPEXP scoInteger scoGetPeriodCounter(ScopeMemory * pScopeMemory, int i);

/**
\brief Get the TraceLength of the Axes i in the ScopeMemory
\param pScopeMemory a pointer on a ScopeMemory
\param i index of an axes
\return trace_length[i]
*/
SCICOS_BLOCKS_IMPEXP scoInteger scoGetLongDrawSize(ScopeMemory * pScopeMemory, int i);

/**
\brief Get the NumberOfSubwin in the ScopeMemory
\param pScopeMemory a pointer on a ScopeMemory
\return number_of_subwin[i]
*/
SCICOS_BLOCKS_IMPEXP scoInteger scoGetNumberOfSubwin(ScopeMemory * pScopeMemory);

/**
\brief Get the NumberOfCurvesBySubwin of Axes i in the ScopeMemory
\param pScopeMemory a pointer on a ScopeMemory
\param i index of an axes
\return number_of_curves_by_subwin[i]
*/
SCICOS_BLOCKS_IMPEXP scoInteger scoGetNumberOfCurvesBySubwin(ScopeMemory * pScopeMemory,int i);

/**
\brief Get the WindowID of the Scope Graphic Window in the ScopeMemory
\param pScopeMemory a pointer on a ScopeMemory
\return win_id
*/
SCICOS_BLOCKS_IMPEXP scoInteger scoGetWindowID(ScopeMemory * pScopeMemory);

/**
\brief Get the BufferSize of the Axes i in the ScopeMemory
\param pScopeMemory a pointer on a ScopeMemory
\param i index of an axes
\return buffer_sze[i]
*/
SCICOS_BLOCKS_IMPEXP scoInteger scoGetShortDrawSize(ScopeMemory * pScopeMemory,int i);

/**
\brief Get the Period of the Axes i in the ScopeMemory
\param pScopeMemory a pointer on a ScopeMemory
\param i index of an axes
\return period[i]
*/
SCICOS_BLOCKS_IMPEXP scoDouble scoGetPeriod(ScopeMemory * pScopeMemory, int i);


/**
\brief Get the Pointer on the ScopeWindow
\param pScopeMemory a pointer on a ScopeMemory
\return an scoGraphicalObject which is the pointer on the Scope Window
*/
SCICOS_BLOCKS_IMPEXP scoGraphicalObject scoGetPointerScopeWindow(ScopeMemory * pScopeMemory);

/**
\brief Get the Pointer on Axes[i]
\param pScopeMemory a pointer on a ScopeMemory
\param i index of an axe
\return an scoGraphicalObject which is the pointer on the Axes[i]
*/
SCICOS_BLOCKS_IMPEXP scoGraphicalObject scoGetPointerAxes(ScopeMemory * pScopeMemory, int i);

/**
\brief Get the Pointer on ShortDraw[i][j]
\param pScopeMemory a pointer on a ScopeMemory
\param i index of an axe
\param j index of a curve in the Axes[i]
\return an scoGraphicalObject which is the pointer on the ShortDraw[i][j]
*/
SCICOS_BLOCKS_IMPEXP scoGraphicalObject scoGetPointerShortDraw(ScopeMemory * pScopeMemory, int i, int j);

/**
\brief Get the Pointer on LongDraw[i][j]
\param pScopeMemory a pointer on a ScopeMemory
\param i index of an axe
\param j index of a curve in the Axes[i]
\return an scoGraphicalObject which is the pointer on the LongDraw[i][j]
*/
SCICOS_BLOCKS_IMPEXP scoGraphicalObject scoGetPointerLongDraw(ScopeMemory * pScopeMemory, int i, int j);

/**
\brief Get status of activation of the block
\return 1 activated or 0 deactivated (by default)

*/
SCICOS_BLOCKS_IMPEXP scoInteger scoGetScopeActivation(ScopeMemory * pScopeMemory);

#endif
