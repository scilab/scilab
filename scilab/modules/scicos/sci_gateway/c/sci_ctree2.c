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
#include "tree.h"
/*--------------------------------------------------------------------------*/
int sci_ctree2(char *fname,unsigned long fname_len)
{
	int one = 1, ipvec = 0,nvec = 0,mvec = 0,noin = 0,moin = 0,ipoin = 0,noinr = 0,moinr = 0,ipoinr = 0;
	int ndep = 0,mdep = 0,ipdep = 0,ndepuptr = 0,mdepuptr = 0,ipdepuptr = 0,ipord = 0,ipok = 0,n = 0,nord = 0;

	CheckRhs(5,5);
	CheckLhs(2,2);

	GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&nvec,&mvec,&ipvec);
	GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&noin,&moin,&ipoin);
	GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&noinr,&moinr,&ipoinr);
	GetRhsVar(4,MATRIX_OF_INTEGER_DATATYPE,&ndep,&mdep,&ipdep);
	GetRhsVar(5,MATRIX_OF_INTEGER_DATATYPE,&ndepuptr,&mdepuptr,&ipdepuptr);
	n = nvec * mvec;
	CreateVar(6,MATRIX_OF_INTEGER_DATATYPE,&n,&one,&ipord);
	CreateVar(7,MATRIX_OF_INTEGER_DATATYPE,&one,&one,&ipok);

	ctree2(istk(ipvec),n,istk(ipdep),istk(ipdepuptr),istk(ipoin),istk(ipoinr),istk(ipord),&nord,istk(ipok));

	*istk(iadr(C2F(intersci).iwhere[5])+1) = nord;

	LhsVar(1) = 6;
	LhsVar(2) = 7;

	PutLhsVar();

	return 0;
}
/*--------------------------------------------------------------------------*/
