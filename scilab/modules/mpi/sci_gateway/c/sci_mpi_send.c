/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2011 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <mpi.h>
#include "api_scilab.h"
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "s_mpi_send.h"
#include "mappingScilabMPI.h"

/******************************************
 * SCILAB function : mpi_send, fin = 1
 ******************************52************/
#define TAG 0
int sci_mpi_send(char *fname,unsigned long fname_len)
{
	int nopt,iopos,m1,n1,l1,m2,n2,m3,n3,m4,n4,l4,un=1,l5;
	char *stringToBeSend;
	double nodeID;
	SciErr sciErr;
        int *piAddr                     = NULL;

	int errorCode = 0;
	mappinpScilabMPI mapping;

	// Tag should be optionnal

	CheckRhs(2,2);	
	CheckLhs(1,1);
	
	mapping=getMPIDataStructure(1);
	getVarAddressFromPosition(pvApiCtx, 2, &piAddr);
	getScalarDouble(pvApiCtx, piAddr ,&nodeID);

//	printf("try: %5.2f, %d, %d, %d, %d\n", mapping.data[0], mapping.rows*mapping.cols, mapping.MPI, TAG, nodeID);
	errorCode = MPI_Send(mapping.data, mapping.rows*mapping.cols, mapping.customMPI, nodeID, TAG, MPI_COMM_WORLD);
    if (errorCode!=MPI_SUCCESS) {
        // TODO : fix that
        printf("Failed : after MPI_Send. Error code: %d\n",errorCode);fflush(NULL);
    }
	
	m3=1;
	n3=1;

	createScalarDouble(pvApiCtx, Rhs + 1, errorCode);
	//	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m3,&n3,&errorCode);
	
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();
	return 0;
}
