/*  Scicos
*
*  Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*  Copyright (C) DIGITEO - 2009 - Allan CORNET
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
#ifndef __CREATEBLKLIST_HXX__
#define __CREATEBLKLIST_HXX__

#include "internal.hxx"

extern "C"
{
#include "scicos_block4.h"
}

/* createblklist : function to create a Typed List
*                 of a scicos_block structure
*                 at the top+1 postion of the stack
*
* needs/depends : var2sci, C2F(mklist), C2F(mtklist),
*                 vvtosci, itosci, string.h, C2F(scierr), str2sci
*
* input argument : Blocks :scicos_block  ptr on a scicos_block structure
*                  ierr : int ptr, an error flag
*                  flag_imp : if flag_imp>=0 then use
*                             import structure for x, xd and g.
*                             In this case flag_imp is the block number.
*                  kfun : the current indexe of the block in the compiled structure
*
* output argument : return 0 if failed, 1 else.
*
*
* 23/06/06, Alan : extracted from sciblk4 to be used
*                  in intgetscicosvars
*
* 26/06/06, Alan : Add flag_imp in input arguments.
*                  This is done to disable scilab crash with getscicosvars("blocks")
*                  because when calling at the beginning of the simulation, x, xd and
*                  g are not yet informed for all blocks with nx!=0 and ng!=0.
*                  (They are not yet called with callf in scicos.c)
*
* 09/02/07, Alan : Add kfun flag and oz/opar
*
*/

types::InternalType* createblklist(const scicos_block* const Blocks, const int flag_imp, const int funtyp);

#endif /* __CREATEBLKLIST_HXX__ */
/*--------------------------------------------------------------------------*/

