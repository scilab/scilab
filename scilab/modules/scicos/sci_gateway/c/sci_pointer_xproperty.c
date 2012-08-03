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
#include "scicos-def.h"
#include "scicos.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
/* variable defined in scicos.c */
extern COSIM_struct C2F(cosim);
/*--------------------------------------------------------------------------*/
int sci_pointer_xproperty(char *fname,unsigned long fname_len)
{
	int isrun = C2F(cosim).isrun;

	if (!isrun) 
	{
		Scierror(999,_("%s: scicosim is not running.\n"),fname);
	}
	else 
	{
		int one = 1;
		int* pointer_xproperty = NULL;
		int n_pointer_xproperty = 0;

		CheckRhs(-1,0);
		CheckLhs(1,1);

		pointer_xproperty = get_pointer_xproperty();
		n_pointer_xproperty = get_npointer_xproperty();

		CreateVarFromPtr(1,MATRIX_OF_INTEGER_DATATYPE,&n_pointer_xproperty,&one,&pointer_xproperty);
		LhsVar(1) = 1;
		PutLhsVar();
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
