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
#include "copyvarfromlistentry.h"
#include "MALLOC.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/ 
extern int *listentry(int *header, int i); /* mexlib.h */
/*--------------------------------------------------------------------------*/ 
int CopyVarFromlistentry(int lw, int *header, int i)
{
	/* Local variable definition*/
	int ret,un=1;
	double *l;
	int n;

	/* Test if we receive a NULL ptr header */
	if (header==NULL) return FALSE;

	/* Get the start address of the i element of the input list*/
	if ((l = (double *) listentry(header,i))==NULL) return FALSE;

	/* Compute the length of the i element in double word */
	n = header[i+2]-header[i+1];

	/* Create empty data of a size n*sizeof(double) at the position lw */
	if ((ret=C2F(createdata)(&lw, n*sizeof(double)))==FALSE) return ret;

	/* Copy the element i to position lw*/
	C2F(unsfdcopy)(&n,l,&un,stk(*Lstk(lw + Top - Rhs)),&un);
	return TRUE;
}
/*--------------------------------------------------------------------------*/ 

