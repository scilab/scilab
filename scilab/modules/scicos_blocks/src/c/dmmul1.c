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
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*
* See the file ./license.txt
*/
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "dynlib_scicos_blocks.h"
/* Table of constant values */
/*--------------------------------------------------------------------------*/
extern int C2F(dmmul1)(double *a, int *na, double *b, int *nb, double *c__,
                       int *nc, int *l, int *m, int *n); /* in elementary_functions */
/*--------------------------------------------------------------------------*/
SCICOS_BLOCKS_IMPEXP int dmmul1(double *a, int *na, double *b, int *nb, double *c__,
                                int *nc, int *l, int *m, int *n)
{
    return C2F(dmmul1)(a, na, b, nb, c__, nc, l, m, n);
} /* dmmul1 */
/*--------------------------------------------------------------------------*/
