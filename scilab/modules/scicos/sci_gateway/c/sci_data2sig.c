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
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
/*--------------------------------------------------------------------------*/
extern int *listentry(int *header, int i);
extern void str2sci(char** x,int n,int m);
extern int C2F(mkmlist)();
/*--------------------------------------------------------------------------*/
/* B=data2sig(A[,step])
*
* Put data in a 'scicos' signal structure.
* If A is a vector, then it will be understand
* as a scalar that evolves along time.
* If A is a matrix, then it will be understand
* as a vector that evolves along time.
* If A is an hypermatrix, then it will be understand
* as a matrix that evolves along time.
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
* Inputs
*   A    : vector,matrix or hypermatrix(with 3 dimensions)
*          of real/complex or integers numbers,
*
*   step : an optional argument that gives the time step
*          for the output time field.
*          If not given, then the time step is 1.
*
* Output
*   B    : a scilab structure that defines a signal :
*          B.values :  A input,
*          B.time   :  the samples time in the form of
*                      a columun vector. The size of that
*                      vector is given by the A input.
*
* initial rev 08/11/07, Alan
*
*/
/*--------------------------------------------------------------------------*/
int sci_data2sig(char *fname, unsigned long fname_len)
{
	int m1 = 0,n1 = 0,it = 0;
	int p1 = 0;
	int *il1 = NULL;
	int m1h = 0,n1h = 0;
	int *ilh1 = NULL;
	int m2h = 0,n2h = 0;
	int *ilh2=NULL;
	int *ilh3=NULL;
	int m2 = 0,n2 = 0,it2 = 0;
	int *il2 = NULL;
	int il = 0, l = 0;
	int l_tmp = 0;

	int i = 0, j = 0, TopSave = 0;

	char *str_stlst[]={"st","dims","values","time"};
	char *str_hmlst[]={"hm","dims","entries"};

	double step_time=1;

	int  ismat=0;

	CheckRhs(1,2);
	CheckLhs(1,1);

	il1 = (int *) GetData(1);
	m1  = il1[1];
	n1  = il1[2];
	it  = il1[3];
	if ((il1[0]!=1) && (il1[0]!=8)) 
	{
		if (il1[0]==17) 
		{
			ilh1 = (int *) (listentry(il1,1));
			m1h  = ilh1[1];
			n1h  = ilh1[2];
			if ((ilh1[m1h*n1h+5]!=17) || (ilh1[m1h*n1h+6]!=22)) 
			{
				Scierror(888,_("%s : First argument must be a matrix or an hypermatrix\nof real, complex or integer numbers.\n"),fname);
				return 0;
			}
			ilh2 = (int *) (listentry(il1,2));
			m2h  = ilh2[1];
			n2h  = ilh2[2];
			if (n2h!=3) 
			{
				Scierror(888, _("%s : Only hypermatrix with 3 dimensions is allowed.\n"), fname);
				return 0;
			}
			m1     = ilh2[4];
			n1     = ilh2[5];
			p1     = ilh2[6];
			ilh3   = (int *) (listentry(il1,3));
			if ((ilh3[0]!=1) && (ilh3[0]!=8)) 
			{
				Scierror(888, _("%s : First argument must be a matrix or an hypermatrix\nof real, complex or integer numbers.\n"),fname);
				return 0;
			}
			it     = ilh3[3];
		}
		else 
		{
			Scierror(888,_("%s : First argument must be a matrix or an hypermatrix\nof real, complex or integer numbers.\n"),fname);
			return 0;
		}
	}

	if (Rhs==2) 
	{
		il2 = (int *) GetData(2);
		m2  = il2[1];
		n2  = il2[2];
		it2 = il2[3];
		if ((m2*n2)!=1) 
		{
			Scierror(888, _("%s : Second argument must be a scalar.\n"),fname);
			return 0;
		}
		if ((il2[0]!=1) || (it2!=0)) 
		{
			Scierror(888, _("%s : Second argument must be a real.\n"),fname);
			return 0;
		}
		step_time = *((double *) &il2[4]);
	}

	TopSave=Top;

	str2sci(str_stlst,1,4);

	Top           = Top + 1;
	il            = iadr(*Lstk(Top));
	l             = sadr(il+4);
	*istk(il)     = 8;
	*istk(il+1)   = 1;
	*istk(il+2)   = 2;
	*istk(il+3)   = 4;
	*istk(il+4)   = 1;
	*istk(il+5)   = 1;
	*Lstk(Top+1)  = l + 1;

	if ((m1!=1) && (n1!=1)) 
	{
		ismat=1;
	}

	switch(il1[0])
	{
	case 1 :
		{
			Top            = Top + 1;
			il             = iadr(*Lstk(Top));
			l              = sadr(il+4);
			*istk(il)      = 1;
			if (ismat) 
			{
				*istk(il+1)  = m1;
				*istk(il+2)  = n1;
			}
			else 
			{
				*istk(il+1)  = n1*m1;
				*istk(il+2)  = 1;
			}
			*istk(il+3)    = it;
			if (it==0) 
			{
				for(i=0;i<(m1*n1);i++) 
				{
					stk(l)[i]  = *((double *)(&il1[4]) + i);
				}
				*Lstk(Top+1) = l + n1*m1;
			}
			else if (it==1) 
			{
				for(i=0;i<(2*m1*n1);i++) 
				{
					stk(l)[i]  = *((double *)(&il1[4]) + i);
				}
				*Lstk(Top+1) = l + 2*n1*m1;
			}
			break;
		}

	case 8 :
		{
			Top            = Top + 1;
			il             = iadr(*Lstk(Top));
			l              = sadr(il+4);
			*istk(il)      = 8;
			if (ismat) 
			{
				*istk(il+1)  = m1;
				*istk(il+2)  = n1;
			}
			else 
			{
				*istk(il+1)  = m1*n1;
				*istk(il+2)  = 1;
			}
			*istk(il+3)    = it;
			if (it==4) 
			{
				for(i=0;i<(m1*n1);i++) 
				{
					*((long *)(istk(il+4)) + i)  = *((long *)(&il1[4]) + i);
				}
				*Lstk(Top+1) = l + (int)(ceil((n1*m1)/2)+1);
			}
			else if (it==2) 
			{
				for(i=0;i<(m1*n1);i++) 
				{
					*((short *)(istk(il+4)) + i)  = *((short *)(&il1[4]) + i);
				}
				*Lstk(Top+1) = l + (int)(ceil((n1*m1)/4)+1);
			}
			else if (it==1) 
			{
				for(i=0;i<(m1*n1);i++) 
				{
					*((char *)(istk(il+4)) + i)  = *((char *)(&il1[4]) + i);
				}
				*Lstk(Top+1) = l + (int)(ceil((n1*m1)/8)+1);
			}
			else if (it==14) 
			{
				for(i=0;i<(m1*n1);i++) 
				{
					*((unsigned long *)(istk(il+4)) + i)  = *((unsigned long *)(&il1[4]) + i);
				}
				*Lstk(Top+1) = l + (int)(ceil((n1*m1)/2)+1);
			}
			else if (it==12) 
			{
				for(i=0;i<(m1*n1);i++) 
				{
					*((unsigned short *)(istk(il+4)) + i)  = *((unsigned short *)(&il1[4]) + i);
				}
				*Lstk(Top+1) = l + (int)(ceil((n1*m1)/4)+1);
			}
			else if (it==11) 
			{
				for(i=0;i<(m1*n1);i++) 
				{
					*((unsigned char *)(istk(il+4)) + i)  = *((unsigned char *)(&il1[4]) + i);
				}
				*Lstk(Top+1) = l + (int)(ceil((n1*m1)/8)+1);
			}
			break;
		}

	case 17 :
		{
			str2sci(str_hmlst,1,3);

			Top           = Top + 1;
			il            = iadr(*Lstk(Top));
			l             = sadr(il+4);
			*istk(il)     = 8;
			*istk(il+1)   = 1;
			*istk(il+2)   = n2h;
			*istk(il+3)   = 4;
			for(i=0;i<(n2h);i++) 
			{
				*((long *)(istk(il+4)) + i)  = *((long *)(&ilh2[4]) + i);
			}
			*Lstk(Top+1) = l + (int)(ceil((n2h)/2)+1);

			switch(ilh3[0])
			{
			case 1 :
				{
					Top            = Top + 1;
					il             = iadr(*Lstk(Top));
					l              = sadr(il+4);
					*istk(il)      = 1;
					*istk(il+1)    = m1*n1*p1;
					*istk(il+2)    = 1;
					*istk(il+3)    = it;
					if (it==0) {
						for(i=0;i<(m1*n1*p1);i++) 
						{
							stk(l)[i]  = *((double *)(&ilh3[4]) + i);
						}
						*Lstk(Top+1) = l + n1*m1*p1;
					}
					else if (it==1) 
					{
						for(i=0;i<(2*m1*n1*p1);i++) 
						{
							stk(l)[i] = *((double *)(&ilh3[4]) + i);
						}
						*Lstk(Top+1) = l + 2*n1*m1*p1;
					}
					break;
				}
			case 8 :
				{
					Top            = Top + 1;
					il             = iadr(*Lstk(Top));
					l              = sadr(il+4);
					*istk(il)      = 8;
					*istk(il+1)    = m1*n1*p1;
					*istk(il+2)    = 1;
					*istk(il+3)    = it;
					if (it==4) 
					{
						for(i=0;i<(m1*n1*p1);i++) 
						{
							*((long *)(istk(il+4)) + i)  = *((long *)(&ilh3[4]) + i);
						}
						*Lstk(Top+1) = l + (int)(ceil((n1*m1*p1)/2)+1);
					}
					else if (it==2) 
					{
						for(i=0;i<(m1*n1*p1);i++) 
						{
							*((short *)(istk(il+4)) + i)  = *((short *)(&ilh3[4]) + i);
						}
						*Lstk(Top+1) = l + (int)(ceil((n1*m1*p1)/4)+1);
					}
					else if (it==1) 
					{
						for(i=0;i<(m1*n1*p1);i++) 
						{
							*((char *)(istk(il+4)) + i)  = *((char *)(&ilh3[4]) + i);
						}
						*Lstk(Top+1) = l + (int)(ceil((n1*m1*p1)/8)+1);
					}
					else if (it==14) 
					{
						for(i=0;i<(m1*n1*p1);i++) 
						{
							*((unsigned long *)(istk(il+4)) + i)  = *((unsigned long *)(&ilh3[4]) + i);
						}
						*Lstk(Top+1) = l + (int)(ceil((n1*m1*p1)/2)+1);
					}
					else if (it==12) 
					{
						for(i=0;i<(m1*n1*p1);i++) 
						{
							*((unsigned short *)(istk(il+4)) + i)  = *((unsigned short *)(&ilh3[4]) + i);
						}
						*Lstk(Top+1) = l + (int)(ceil((n1*m1*p1)/4)+1);
					}
					else if (it==11) 
					{
						for(i=0;i<(m1*n1*p1);i++) 
						{
							*((unsigned char *)(istk(il+4)) + i)  = *((unsigned char *)(&ilh3[4]) + i);
						}
						*Lstk(Top+1) = l + (int)(ceil((n1*m1*p1)/8)+1);
					}
					break;
				}
			}
			C2F(mkmlist)((i=3,&i));
			break;
		}
	}

	Top           = Top + 1;
	il            = iadr(*Lstk(Top));
	l             = sadr(il+4);
	*istk(il)     = 1;
	if (il1[0]==17) 
	{
		*istk(il+1)   = p1;
	}
	else 
	{
		if (ismat) 
		{
			*istk(il+1) = m1;
		}
		else 
		{
			*istk(il+1) = n1*m1;
		}
	}
	*istk(il+2)   = 1;
	*istk(il+3)   = 0;
	for(i=0;i<(*istk(il+1));i++) 
	{
		stk(l)[i]  = (double) i*step_time;
	}
	*Lstk(Top+1) = l + *istk(il+1);

	C2F(mkmlist)((i=4,&i));

	Top = TopSave;

	i = 4;
	j = 1;
	if (Rhs == 2) 
	{
		CreateVar(3,MATRIX_ORIENTED_TYPED_LIST_DATATYPE,&i,&j,&l_tmp);
		LhsVar(1) = 3;
	}
	else 
	{
		CreateVar(2,MATRIX_ORIENTED_TYPED_LIST_DATATYPE,&i,&j,&l_tmp);
		LhsVar(1) = 2;
	}

	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
