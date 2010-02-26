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
#include "HandleManagement.h" /* sciGetPointerFromHandle */
#include "Scierror.h"
#include "scoMisc.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
/* permutobj : concurrent version
* of swap_handles for scicos editor.
*
* Copyright INRIA
* A. Layec/S. Mannori - 13/12/06
*/


/* intpermutobj_c
*
* -->permutobj(hdl1,hdl2);
*
* exchange graphicals objects
* of two children handles
*
* hdl1,hdl2 : valid children handles
*             in valid parent handle(s)
*
* no outputs
*
* Alan-02/12/06, initial rev.
*/
int sci_permutobj(char *fname,unsigned long fname_len)
{
	/*local variables declaration*/
	unsigned long hdl_1, hdl_2;
	sciPointObj *pobj_1 = NULL, *pparent_1 = NULL;
	sciPointObj *pobj_2 = NULL, * pparent_2 = NULL;
	static int m1 = 0, n1 = 0, l1 = 0;
	static int m2 = 0, n2 = 0, l2 = 0;

	/*check number of rhs parameters*/
	CheckRhs(2,2);

	/*get/check rhs 1*/
	GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE,&m1,&n1,&l1);
	if ( m1!=1 || n1!=1 ) 
	{
		Scierror(999,_("%s : Bad size for Rhs(1).\n"),fname);
		return 0; 
	}
	hdl_1 = (unsigned long)*hstk(l1);
	pobj_1 = sciGetPointerFromHandle(hdl_1);
	if (pobj_1 == NULL) 
	{
		Scierror(999,_("%s :the handle is not or no more valid.\n"),fname);
		return 0; 
	}
	pparent_1 = sciGetParentSubwin(pobj_1);

	/*get/check rhs 2*/
	GetRhsVar(2, GRAPHICAL_HANDLE_DATATYPE,&m2,&n2,&l2);
	if (m2!=1 || n2!=1) 
	{
		Scierror(999,_("%s : Bad size for Rhs(2).\n"),fname);
		return 0; 
	}
	hdl_2 = (unsigned long)*hstk(l2);
	pobj_2 = sciGetPointerFromHandle(hdl_2);
	if (pobj_2 == NULL) 
	{
		Scierror(999,_("%s :the handle is not or no more valid.\n"),fname);
		return 0; 
	}
	pparent_2 = sciGetParentSubwin(pobj_2);

	/*call permutobj*/
	permutobj(pobj_1,pparent_1,pobj_2,pparent_2);

	C2F(putlhsvar)();
	/* end */
	return 0;
}
/*--------------------------------------------------------------------------*/ 
