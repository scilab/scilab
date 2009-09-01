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
\file scoMisc.h
\author Benoit Bayol
\version 1.0
\date September 2006 - January 2007
\brief Header File of the scoMisc.c file 
*/

#ifndef __SCO_MISC_H__
#define __SCO_MISC_H__

#include "dynlib_scicos_blocks.h"
#include"scoBase.h"

/**
\brief Stop simulation and return an indication of the bug
\param pScopeMemory a pointer on a ScopeMemory
\param code_error a code error (see in the function to determine the right number for the moment)
*/
SCICOS_BLOCKS_IMPEXP void scoScopeError(ScopeMemory * pScopeMemory, int code_error);

/**
*/
SCICOS_BLOCKS_IMPEXP int permutobj(sciPointObj *pobj_1, sciPointObj *pparent_1,
			  sciPointObj *pobj_2, sciPointObj *pparent_2);
#endif
