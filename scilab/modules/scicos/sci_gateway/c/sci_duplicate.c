/*  Scicos
*
*  Copyright (C) INRIA - Allan CORNET
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
/* INRIA 2008 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "gw_scicos.h"
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
static void duplicata(int *n,double *v,double *w,double *ww,int *nw);
static void comp_size(double *v,int *nw,int n);
/*--------------------------------------------------------------------------*/
int sci_duplicate(char *fname,unsigned long fname_len)
{
	int m1 = 0,m2 = 0,m3 = 0,n1 = 0,n2 = 0,n3 = 0,l1 = 0,l2 = 0,l3 = 0,n = 0;

	CheckRhs(2,2);
	CheckLhs(1,1);

	GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
	GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);

	n = m1*n1;
	if (n == 0) 
	{
		m3 = 0;
		CreateVar(3, MATRIX_OF_DOUBLE_DATATYPE, &m3, &m3, &l3);
		LhsVar(1) = 3;
		PutLhsVar();
		return 0;
	}

	if (n != m2*n2) 
	{
		Scierror(999,_("%s: 1st and 2nd argument must have equal size\n"),fname);
		return 0;
	}

	comp_size(stk(l2),&n3,n);
	m3 = 1;
	CreateVar(3, MATRIX_OF_DOUBLE_DATATYPE, &n3, &m3, &l3);
	duplicata(&n,stk(l1),stk(l2),stk(l3),&n3);

	LhsVar(1) = 3;
	PutLhsVar();

	return 0;
}
/*--------------------------------------------------------------------------*/ 
static void duplicata(int *n,double *v,double *w,double *ww,int *nw)
{
	int i,j,k;
	k=0;
	for (i=0;i<*n;i++) {
		for (j=0;j<(int) w[i];j++) {
			ww[k]=v[i];
			k=k+1;
		}
	}
	*nw=k;
}
/*--------------------------------------------------------------------------*/ 
static void comp_size(double *v,int *nw,int n)
{
	int i;
	*nw=0;
	for (i=0;i<n;i++) {
		if (v[i]>0) *nw=*nw+(int) v[i];
	}
}
/*--------------------------------------------------------------------------*/ 
