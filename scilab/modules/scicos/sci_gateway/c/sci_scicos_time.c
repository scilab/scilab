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
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
* See the file ./license.txt
*/

/*--------------------------------------------------------------------------*/
#include "gw_scicos.h"
#include "stack-c.h"
#include "scicos.h"
/*--------------------------------------------------------------------------*/
int sci_scicos_time(char *fname,unsigned long fname_len)
{
	int un = 1,l1 = 0;

	CheckRhs(-1, 0);
	CheckLhs(1, 1);

	CreateVar(1, MATRIX_OF_DOUBLE_DATATYPE,(un=1,&un),(un=1,&un),&l1);

	*stk(l1) = get_scicos_time();

	LhsVar(1) = 1;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/