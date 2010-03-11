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
#include "s_mpi_recv.h"
#include "stackTypeVariable.h"
#include "MALLOC.h"
/******************************************
 * SCILAB function : mpi_recv, fin = 2
 ******************************************/

int sci_mpi_recv (char *fname,unsigned long fname_len)
{
	int nopt,iopos,m1,n1,l1,m2,n2,l2,m3,n3,l3,ne4,me4,nn4,un=1,mn4,l4,l5,lrs,mn;
	static rhs_opts opts[]={
		{-1,"comm","i",0,0,0},
		{-1,NULL,NULL,NULL,0,0}};
	static int xdat0[]= {MPI_COMM_WORLD}, *dat0 = xdat0;
	double *le4;
	int rankSource;
	int tag;
	char *stringToBeReceived=(char*)MALLOC((256)*sizeof(char));

	CheckRhs(2,2);	
	CheckLhs(1,1);

	GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
	CheckScalar(1,m1,n1);
	rankSource=(int) *stk(l1);

	GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE,&m2,&n2,&l2);
	CheckScalar(2,m2,n2);
	tag=(int) *stk(l2);

	int msgsize;
	MPI_Status status;
	/* Freemat envoit d'abord le nombre de message puis le message ?! a voir si c'est obligatoire ou pas */
	//	MPI_Recv(&msgsize,1,MPI_INT,rankSource,tag,MPI_COMM_WORLD,&status);
	//	void *cp = malloc(msgsize);
	MPI_Recv(stringToBeReceived,255,MPI_CHAR,rankSource,tag,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	//	MPI_Recv(buff, BUFSIZE, MPI_CHAR, 0, TAG, MPI_COMM_WORLD, &stat);

	m3=1;
	n3=1;
	CreateVarFromPtr( Rhs+1, "c",(m3=(int)strlen(stringToBeReceived), &m3),&n3,&stringToBeReceived);
	
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();	
	FREE(stringToBeReceived);

	return 0;
}
