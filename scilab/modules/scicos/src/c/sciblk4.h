/*  Scicos
*
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
#ifndef __SCIBLK4_H__
#define __SCIBLK4_H__

#include "scicos_block4.h"

/* sciblk4. Run scilab block type 5.
*
* Input parameters :
* Blocks : Tlist
*    - 1 : Blocks(1)      : !scicos_block nevprt funpt type   scsptr  nz
*                            z            nx     x     xd     res     nin
*                            insz         inptr  nout  outsz  outptr  nevout
*                            evout        nrpar  rpar  nipar  ipar    ng
*                            g            ztyp   jroot label  work    nmode
*                            mode !
*    - 2  : Blocks.nevprt  :
*    - 3  : Blocks.funpt   :
*    - 4  : Blocks.type    :
*    - 5  : Blocks.scsptr  :
*    - 6  : Blocks.nz      :
*    - 7  : Blocks.z       :
*    - 8  : Blocks.noz     :
*    - 9  : Blocks.ozsz    :
*    - 10 : Blocks.oztyp   :
*    - 11 : Blocks.oz      :
*    - 12 : Blocks.nx      :
*    - 13 : Blocks.x       :
*    - 14 : Blocks.xd      :
*    - 15 : Blocks.res     :
*    - 16 : Blocks.nin     :
*    - 17 : Blocks.insz    :
*    - 18 : Blocks.inptr   :
*    - 19 : Blocks.nout    :
*    - 20 : Blocks.outsz   :
*    - 21 : Blocks.outptr  :
*    - 22 : Blocks.nevout  :
*    - 23 : Blocks.evout   :
*    - 24 : Blocks.nrpar   :
*    - 25 : Blocks.rpar    :
*    - 26 : Blocks.nipar   :
*    - 27 : Blocks.ipar    :
*    - 28 : Blocks.nopar   :
*    - 29 : Blocks.oparsz  :
*    - 30 : Blocks.opartyp :
*    - 31 : Blocks.opar    :
*    - 32 : Blocks.ng      :
*    - 33 : Blocks.g       :
*    - 34 : Blocks.ztyp    :
*    - 35 : Blocks.jroot   :
*    - 36 : Blocks.label   :
*    - 37 : Blocks.work    :
*    - 38 : Blocks.nmode   :
*    - 39 : Blocks.mode    :
*
* flag : integer
*         0 : update continuous state
*         1 : update output state
*         2 : update state
*         3 : update event output state
*         4 : state initialisation
*         5 : finish
*         6 : output state initialisation
*         7 : define property of continuous time states
*         9 : zero crossing surface computation
*        10 : Jacobian computation
*
* Output parameters :
* Blocks : Scilab Tlist (updated blocks Scilab list)
*
* 08/06/06, Alan   : Rewritten from original code of sciblk4.c
*                    of scicos 2.7.
* 09/02/07, Alan   : Add oz as a discrete state
*                    Update taking to account implicit/explicit call of simulator
*                    for some output elements
*/

void sciblk4(scicos_block *Blocks, const int flag);

#endif /* __SCIBLK4_H__ */
/*--------------------------------------------------------------------------*/
