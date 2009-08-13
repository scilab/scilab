/**
 * Function calling mpi_comm_size
 * @author Sylvestre LEDRU - INRIA
 * @date 2007
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
			getVarAddressFromPosition(1, &piAddr);

			if (getVarType(piAddr)==sci_matrix)
				{
					//					GetRhsVar(1,"d",&m1,&n1,&l1);
					comm = (MPI_Comm)(int) getMatrixOfDouble(piAddr, &iRows, &iCols, &pdblReal);

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
	
	int iRet = createMatrixOfDouble(Rhs + 1, iRows2, iCols2, pdblReal1);
    if(iRet)
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
