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
#include "stack-c.h"
#include "api_scilab.h"
#include "Scierror.h"
#include <stdio.h>
/**
 * SCILAB function : mpi_comm_size, fin = 3
 * This function returns the rank of a process 
 */
int sci_mpi_comm_size(char *fname,unsigned long fname_len)
{
	int comm_size;
	MPI_Comm comm=NULL;
    int *piAddr         = NULL;
    int iRows           = 1;
    int iCols           = 1;
    int iRows2           = 1;
    int iCols2           = 1;
    double* pdblReal    = NULL;

	CheckRhs(0,1); // Check the parameters of the function ... Here 0 or 1
	CheckLhs(1,1); // The output of the function (1 parameter)
	if (Rhs==1)
		{
			int typevar;
			getVarAddressFromPosition(pvApiCtx, 1, &piAddr);

			getVarType(pvApiCtx, piAddr, &typevar);
			if (typevar == sci_matrix)
				{
					//					GetRhsVar(1,"d",&m1,&n1,&l1);
					getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &pdblReal);
					comm=(MPI_Comm)(int) pdblReal;

					//					comm=(MPI_Comm)(int)istk(l1);
				}else{
					Scierror(999,"The first parameter should be an int");
				}
		}
	else
		{
			comm=MPI_COMM_WORLD;
		}
	printf("comm : %d\n",comm);
	MPI_Comm_size(comm, &comm_size);
	printf("comm_size : %d\n",comm_size);

    double *pdblReal1 = (double*)malloc(sizeof(double) * iRows2 * iCols2);
	pdblReal1[0]=(double)comm_size;
	
	SciErr iRet = createMatrixOfDouble(pvApiCtx, Rhs + 1, iRows2, iCols2, pdblReal1);
    if(iRet.iErr)
    {
		Scierror(999,"error in the creation of the variable");
    }
	free(pdblReal1);
	//	CreateVar(1, "d", &m1, &n1 ,&l1); // Create the space in the stack for comm_size
	//	*stk(l1)=(double)comm_size; // Copy comm_size into the stack

	LhsVar(1)= Rhs + 1;
	C2F(putlhsvar)();
	return 0;
}
