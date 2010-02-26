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
#include "tree.h"
/*--------------------------------------------------------------------------*/
int sci_ctree4(char *fname,unsigned long fname_len)
{
	int one=1,ipvec = 0,nvec = 0,mvec = 0,noin = 0,moin = 0,ipoin = 0,noinr = 0,moinr = 0,ipoinr = 0;
	int nnd = 0,mnd = 0,ipnd = 0,ntyp = 0,mtyp = 0,iptyp = 0,ipr1 = 0,ipr2 = 0,n = 0,nr = 0,nn = 0;

	CheckRhs(5,5);
	CheckLhs(2,2);

	GetRhsVar(1, MATRIX_OF_INTEGER_DATATYPE,&nvec,&mvec,&ipvec);
	GetRhsVar(2, MATRIX_OF_INTEGER_DATATYPE,&noin,&moin,&ipoin);
	GetRhsVar(3, MATRIX_OF_INTEGER_DATATYPE,&noinr,&moinr,&ipoinr);
	GetRhsVar(4, MATRIX_OF_INTEGER_DATATYPE,&nnd,&mnd,&ipnd);
	GetRhsVar(5, MATRIX_OF_INTEGER_DATATYPE,&ntyp,&mtyp,&iptyp);

	n = nvec * mvec;
	nn = nnd * mnd;

	CreateVar(6, MATRIX_OF_INTEGER_DATATYPE,&one,&nn,&ipr1);
	CreateVar(7, MATRIX_OF_INTEGER_DATATYPE,&one,&nn,&ipr2);

	ctree4(istk(ipvec),n,istk(ipnd),mnd,istk(iptyp),istk(ipoin),istk(ipoinr),istk(ipr1),istk(ipr2),&nr);

	LhsVar(1) = 6;
	LhsVar(2) = 7;

	/*      nbcols(6)=nr */
	*istk(iadr(C2F(intersci).iwhere[5])+2) = nr;

	/*      nbcols(7)=nr */
	*istk(iadr(C2F(intersci).iwhere[6])+2) = nr;

	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
