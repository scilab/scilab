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
\file scoMemoryScope.h
\author Benoit Bayol
\version 1.0
\date September 2006 - January 2007
\brief Header file of the scoMemoryScope.c file
*/

#ifndef __SCO_MEMORYSCOPE_H__
#define __SCO_MEMORYSCOPE_H__

#include "dynlib_scicos_blocks.h"
#include"scoBase.h"

/**
\brief Initialization with scicos_malloc on the ScopeMemory and put in ScopeMemory the number_of_subwin and the number_of_curves_by_subwin
\param block a pointer on a scicos_block, typically the bloc in agument of the computational function
\param pScopeMemory a pointer on a pointer of a ScopeMemory, typically &pScopeMemory where pScopeMemory is a ScopeMemory *
\param number_of_subwin Number of Subwin in the whole Scope
\param number_of_curves_by_subwin Number of Curves for the subwin[i]
*/
SCICOS_BLOCKS_IMPEXP void scoInitScopeMemory(void ** block_work, ScopeMemory ** pScopeMemory, int number_of_subwin, int * number_of_curves_by_subwin);

/**
\brief Retrieve the whole ScopeMemory and put in the pScopeMemory
\param block a pointer on a scicos_block, typically the bloc in agument of the computational function
\param pScopeMemory a pointer on a pointer of a ScopeMemory, typically &pScopeMemory where pScopeMemory is a ScopeMemory *
*/
SCICOS_BLOCKS_IMPEXP void scoRetrieveScopeMemory(void ** block_work, ScopeMemory ** pScopeMemory);

/**
\brief Free the ScopeMemory which has been allocated by scicos_malloc in the scoInitScopeMemory
\param block a pointer on a scicos_block, typically the bloc in agument of the computational function
\param pScopeMemory a pointer on a pointer of a ScopeMemory, typically &pScopeMemory where pScopeMemory is a ScopeMemory *
*/
SCICOS_BLOCKS_IMPEXP void scoFreeScopeMemory(void ** block_work, ScopeMemory ** pScopeMemory);

/**
\brief Realloc to extend the size of the longdraw in the memory
\param pLongDraw A pointer on the longdraw which needs to be reallocated
\param NbrPtsLong Current size of the longdraw
\param buffer_size Buffer size which would be added to the longdraw
\param plus An other buffer size which is a security
*/
SCICOS_BLOCKS_IMPEXP void scoReallocLongDraw(scoGraphicalObject pLongDraw, int NbrPtsLong, int buffer_size, int plus);

#endif
