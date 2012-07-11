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
#include "gw_scicos.h"
#include "stack-c.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int sci_diffobjs(char *fname,unsigned long fname_len)
{
	int one = 1, l3 = 0,k = 0;
	int size1 = 0;
	int size2 = 0;
	int *header1 = NULL;
	int *header2 = NULL;

	CheckRhs(2,2);
	CheckLhs(1,1);

	header1 = GetData(1);
	header2 = GetData(2);

	CreateVar(3,MATRIX_OF_DOUBLE_DATATYPE,(one=1,&one),(one=1,&one),&l3);

	LhsVar(1) = 3;

	size1 = 2*(*Lstk(Top-Rhs+2)-*Lstk(Top-Rhs+1));
	size2 = 2*(*Lstk(Top-Rhs+3)-*Lstk(Top-Rhs+2));

	if (size1 != size2) 
	{
		*stk(l3)=1;
		PutLhsVar();
		return 0;
	}

	for (k=0; k<size1; k++) 
	{
		if (header1[k] != header2[k]) 
		{
			*stk(l3)=1;
			PutLhsVar();
			return 0;
		}
		*stk(l3)=0;
	}
	PutLhsVar();

	return 0;
}
/*--------------------------------------------------------------------------*/
