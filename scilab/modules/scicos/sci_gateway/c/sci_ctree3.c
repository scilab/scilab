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
int sci_ctree3(char *fname,unsigned long fname_len)
{
	int one=1,ipvec = 0,nvec = 0,mvec = 0,ntyp = 0,mtyp = 0,iptyp = 0,nbex = 0,mbex = 0,ipbex = 0;
	int ndep = 0,mdep = 0,ipdep = 0,ndepuptr = 0,mdepuptr = 0,ipdepuptr = 0,ipord = 0,ipok = 0,n = 0,nord = 0;
	int nbop = 0,mbop = 0,ipbop = 0,nbln = 0,mbln = 0,ipbln = 0,nblr = 0,mblr = 0,ipblr = 0;

	CheckRhs(8,8);
	CheckLhs(2,2);

	GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&nvec,&mvec,&ipvec);
	GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&ndep,&mdep,&ipdep);
	GetRhsVar(3,MATRIX_OF_INTEGER_DATATYPE,&ndepuptr,&mdepuptr,&ipdepuptr);
	GetRhsVar(4,MATRIX_OF_INTEGER_DATATYPE,&ntyp,&mtyp,&iptyp);
	GetRhsVar(5,MATRIX_OF_INTEGER_DATATYPE,&nbex,&mbex,&ipbex);
	GetRhsVar(6,MATRIX_OF_INTEGER_DATATYPE,&nbop,&mbop,&ipbop);
	GetRhsVar(7,MATRIX_OF_INTEGER_DATATYPE,&nbln,&mbln,&ipbln);
	GetRhsVar(8,MATRIX_OF_INTEGER_DATATYPE,&nblr,&mblr,&ipblr);

	n=nvec*mvec;
	CreateVar(9,MATRIX_OF_INTEGER_DATATYPE,&n,&one,&ipord);
	CreateVar(10,MATRIX_OF_INTEGER_DATATYPE,&one,&one,&ipok);

	ctree3(istk(ipvec),n,istk(ipdep),istk(ipdepuptr),istk(iptyp),istk(ipbex),
		istk(ipbop),istk(ipbln),istk(ipblr),istk(ipord),&nord,istk(ipok));

	*istk(iadr(C2F(intersci).iwhere[8])+1) = nord;

	LhsVar(1) = 9;
	LhsVar(2) = 10;

	PutLhsVar();

	return 0;
}
/*--------------------------------------------------------------------------*/
