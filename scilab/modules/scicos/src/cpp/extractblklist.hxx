/*  Scicos
*
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*  Copyright (C) DIGITEO - Allan CORNET
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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/

#ifndef __EXTRACTBLKLIST_HXX__
#define __EXTRACTBLKLIST_HXX__

#include "tlist.hxx"

extern "C"
{
#include "scicos_block4.h"
}
/* extractblklist : create a scicos_block C structure from
* a scicos_block scilab structure.
*
* Input : il : the ptr of the scicos_block scilab structure
*              in the stack
*
* Output : Block : C scicos_block structure
*          ierr  : an error number (ifany)
*
* initial rev 13/11/07, Alan Layec
*/

bool extractblklist(types::TList* t, scicos_block* const Block);

#endif /* __EXTRACTBLKLIST_HXX__ */
/*--------------------------------------------------------------------------*/
