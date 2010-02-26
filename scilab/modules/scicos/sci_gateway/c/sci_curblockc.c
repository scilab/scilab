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
/* INRIA 2008 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "gw_scicos.h"
#include "stack-c.h"
#include "scicos-def.h"
/*--------------------------------------------------------------------------*/
/* intcurblkc curblock interface routine
*
* [nblk]=curblock()
*
* rhs : empty
* lhs : nblk : the current block (double)
*
* 20/06/06, Alan : Rewritten from original fortran
* source code intcurblk in coselm.f.
*
* 12/02/07 : update lhs to double output
*
*/
int sci_curblockc(char *fname,unsigned long fname_len)
{
	/***********************
	* variables declaration
	***********************/

	extern CURBLK_struct C2F(curblk);

	/* address of the data of the output parameter */
	int l1 = 0;
	/* local counter variable */
	int j = 0, k = 0;
	/* definition of min/max output argument */
	static int minlhs = 1, maxlhs = 1;

	/**************************
	* Check number of  outputs
	**************************/
	CheckLhs(minlhs, maxlhs);

	/************************
	* Create double variable
	************************/
	/* Create int32 variable at the top addr. of the stack */
	CreateVar(1,MATRIX_OF_DOUBLE_DATATYPE,(j = 1,&j),(k = 1,&k),&l1);

	/* Store value of C2F(curblk).kfun at the l1 address in istk */
	*stk(l1) = (double)C2F(curblk).kfun;

	/* return the value stored at Top address to lhs variable */
	LhsVar(1) = 1;
	C2F(putlhsvar)();

	/* return 0 as default value */
	return 0;
}
/*--------------------------------------------------------------------------*/ 
