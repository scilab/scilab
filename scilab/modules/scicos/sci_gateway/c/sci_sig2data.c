/*  Scicos
*
*  Copyright (C) INRIA - 
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
#include <stdio.h>
#include "gw_scicos.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "copyvarfromlistentry.h"
/*--------------------------------------------------------------------------*/
extern int *listentry(int *header, int i);
/*--------------------------------------------------------------------------*/
/* [x[,t]]=sig2data(B)
*
* Extract data from a 'scicos' signal structure.
*  x will be the field B.values
*  t will be the field B.time
*
* Usage :
* -->a(:,:,4)=([7 8 9;10 11 12]);
* -->b=data2sig(a,0.1)
* b  =
*   values: [2x3x4 hypermat]
*   time: [4x1 constant]
* -->b.time
* ans  =
*    0.
*    0.1.
*    0.2.
*    0.3.
*
* -->[x,t]=sig2data(b)
* t  =
*  0.
*   0.1
*   0.2
*   0.3
* x  =
* (:,:,1)
*
*  0.    0.    0.
*  0.    0.    0.
*
* Inputs
*   B    : a scilab structure that defines a signal :
*          B.values :  B data,
*          B.time   :  the samples time in the form of
*                      a columun vector.
*
* Outputs
*   x    : the field B.values
*   t    : the field B.time
*
* initial rev 08/11/07, Alan
*
*/
int sci_sig2data(char *fname, unsigned long fname_len)
{
	int m1 = 0, n1 = 0;
	int *il1 = NULL;
	int m1h = 0, n1h = 0;
	int *ilh1 = NULL;
	int ierr = 0;

	CheckRhs(1,1);
	CheckLhs(1,2);

	il1 = (int *) GetData(1);
	m1  = il1[1];
	n1  = il1[2];

	if (il1[0] != sci_mlist) 
	{
		Scierror(888,_("%s : First argument must be a structure.\n"),fname);
		return 0;
	}

	ilh1 = (int *) (listentry(il1,1));
	m1h  = ilh1[1];
	n1h  = ilh1[2];
	if ((ilh1[m1h*n1h+5]!=28) || (ilh1[m1h*n1h+6]!=29)) 
	{
		Scierror(888,_("%s : First argument must be a structure.\n"),fname);
		return 0;
	}

	if ((n1h!=4) || \
		(ilh1[m1h*n1h+11]!=31) || \
		(ilh1[m1h*n1h+12]!=10) || \
		(ilh1[m1h*n1h+13]!=21) || \
		(ilh1[m1h*n1h+14]!=30) || \
		(ilh1[m1h*n1h+15]!=14) || \
		(ilh1[m1h*n1h+16]!=28) || \
		(ilh1[m1h*n1h+17]!=29) || \
		(ilh1[m1h*n1h+18]!=18) || \
		(ilh1[m1h*n1h+19]!=22) || \
		(ilh1[m1h*n1h+20]!=14)) 
	{
			Scierror(888, _("%s : First argument must be a scicos signal structure.\n"),fname);
			return 0;
	}

	ierr = CopyVarFromlistentry(Top-Rhs+2,il1,3);
	LhsVar(1) = 2;

	if (Lhs==2) 
	{
		ierr = CopyVarFromlistentry(Top-Rhs+3,il1,4);
		LhsVar(2) = 3;
	}

	PutLhsVar();

	return 0;
}
/*--------------------------------------------------------------------------*/
