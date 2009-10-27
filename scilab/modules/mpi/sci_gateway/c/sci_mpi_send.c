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
#include <mpi.h>
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "s_mpi_send.h"
#include "stackTypeVariable.h"

/******************************************
 * SCILAB function : mpi_send, fin = 1
 ******************************************/
#define TAG 0
int sci_mpi_send(char *fname,unsigned long fname_len)
{
	int nopt,iopos,m1,n1,l1,m2,n2,l2,m3,n3,m4,n4,l4,un=1,l5;
	static rhs_opts opts[]={
		{-1,"comm","i",0,0,0},
		{-1,NULL,NULL,NULL,0,0}};
	static int xcomm_world[]= {MPI_COMM_WORLD}, *comm_world = xcomm_world;
	char *stringToBeSend;
	int nodeID;

	int one = 1;
	int l3 = 0;


	// Tag should be optionnal

	CheckRhs(2,2);	
	CheckLhs(1,1);

	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	//	CheckScalar(1,m1,n1);
	stringToBeSend=cstk(l1);


	GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
	CheckScalar(2,m2,n2);
	nodeID=(int) *stk(l2);

	MPI_Send(stringToBeSend, strlen(stringToBeSend), MPI_CHAR, nodeID, TAG, MPI_COMM_WORLD);
	
	m3=1;
	n3=1;
	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m3,&n3,&l3);
	*stk(l3) = 1;
	
	LhsVar(1) = Rhs+1;

	C2F(putlhsvar)();

	return 0;
}
