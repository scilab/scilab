/**
 * Function calling mpi_comm_size
 * @author Sylvestre LEDRU - INRIA
 * @date 2007
 */
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "stack-c.h"

/**
 * SCILAB function : mpi_comm_size, fin = 3
 * This function returns the rank of a process 
 */
int C2F(sci_mpi_comm_size) (char *fname,unsigned long fname_len)
{
	int comm_size;
	int n1=1,m1=1,l1=0;
	MPI_Comm comm=NULL;
	CheckRhs(0,1); // Check the parameters of the function ... Here 0 or 1
	CheckLhs(1,1); // The output of the function (1 parameter)
	if (Rhs==1)
		{
			if (GetType(1)==sci_matrix)
				{
					GetRhsVar(1,"d",&m1,&n1,&l1);
					comm=(MPI_Comm)(int)istk(l1);
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

	CreateVar(1, "d", &m1, &n1 ,&l1); // Create the space in the stack for comm_size
	*stk(l1)=(double)comm_size; // Copy comm_size into the stack

	LhsVar(1)= 1;
	C2F(putlhsvar)();
	return 0;
}
