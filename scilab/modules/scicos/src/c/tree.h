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

#ifndef __TREE_H__
#define __TREE_H__
/*--------------------------------------------------------------------------*/

int ctree2(int* vect, int nb, int* deput, int* depuptr, int* outoin, int* outoinptr,
           int* ord, int* nord, int* ok);

int ctree3(int*vec, int nb, int* depu, int* depuptr, int* typl, int* bexe, int* boptr,
           int* blnk, int* blptr, int* ord, int* nord, int* ok);

int ctree4(int* vec, int nb, int *nd, int nnd, int* typ_r, int* outoin,
           int* outoinptr, int* r1, int* r2, int* nr);

/*--------------------------------------------------------------------------*/
#endif /* __TREE_H__ */
