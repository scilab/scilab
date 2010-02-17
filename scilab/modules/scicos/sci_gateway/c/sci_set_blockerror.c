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
#include "scicos-def.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern COSIM_struct C2F(cosim);
/*--------------------------------------------------------------------------*/
int sci_set_blockerror(char *fname,unsigned long fname_len)
{
	int one = 1, l1 = 0;
	int isrun = C2F(cosim).isrun;

	CheckRhs(1,1);

	if (!isrun) 
	{
		Scierror(999, _("%s: scicosim is not running.\n"),fname);
	}
	else 
	{
		GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE,(one=1,&one),(one=1,&one),&l1);
		set_block_error(*istk(l1));

		LhsVar(1) = 0;
		C2F(putlhsvar)();
	}
	return 0;
}
/*--------------------------------------------------------------------------*/ 
