/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2009 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "s_mpi_isend.h"

/******************************************
 * SCILAB function : mpi_isend, fin = 3
 ******************************************/

int intsmpi_isend (char *fname,unsigned long fname_len)
{
	int nopt,iopos,m1,n1,l1,m2,n2,l2,m3,n3,l3,m4,n4,l4,un=1,l5,l6;
	static rhs_opts opts[]={
		{-1,"comm","i",0,0,0},
		{-1,NULL,NULL,NULL,0,0}};
	static int xdat0[]= {MPI_COMM_WORLD}, *dat0 = xdat0;
	nopt=NumOpt();
	CheckRhs(3,3+nopt);
	CheckLhs(1,2);
	/*  checking variable buff */
	GetRhsVar(1,"d",&m1,&n1,&l1);
	/*  checking variable dest */
	GetRhsVar(2,"i",&m2,&n2,&l2);
	CheckScalar(2,m2,n2);
	/*  checking variable tag */
	GetRhsVar(3,"i",&m3,&n3,&l3);
	CheckScalar(3,m3,n3);
	iopos=Rhs;
	if ( get_optionals(fname,opts) == 0) return 0;
	/* default value to optional argument comm */
	if ( opts[0].position == -1 ){
		iopos++ ; opts[0].position = iopos;
		opts[0].m = 1;opts[0].n = 1;
		CreateVarFromPtr(opts[0].position,opts[0].type,&opts[0].m,&opts[0].n,&dat0);
		opts[0].l = VarPtr(opts[0].position);
	}
	else { 
		GetRhsVar(4,"i",&m4,&n4,&l4);
		CheckScalar(4,m4,n4);
	} 
	/* cross variable size checking */
	CreateVar(5,"i",&un,&un,&l5);/* named: request */
	CreateVar(6,"i",&un,&un,&l6);/* named: res */
	s_mpi_isend(stk(l1),&m1,&n1,istk(l2),istk(l3),istk(opts[0].l),istk(l5),istk(l6));
	LhsVar(1)= 5;
	LhsVar(2)= 6;
	return 0;
}
