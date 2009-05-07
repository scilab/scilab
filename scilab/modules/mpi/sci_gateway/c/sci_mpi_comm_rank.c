/**
 * Function calling mpi_comm_rank
 * @author Sylvestre LEDRU INRIA
 * @date 2007
 */
#include "gw_mpi.h"
#include "sci_mpi.h"

/**
 * SCILAB function : mpi_comm_rank, fin = 7
 * This function returns the rank of a process within
 * the specified communicator.
 */

int C2F(sci_mpi_comm_rank) (char *fname,unsigned long fname_len)
{
	int comm_rank;
	int n1=1,m1=1,l1=0;
	CheckRhs(0,0); // Check the parameters of the function ... Here 0
	CheckLhs(1,1); // The output of the function (1 parameter)
	MPI_Comm_rank(MPI_COMM_WORLD, &comm_rank);
	
	CreateVar(1, "d", &m1, &n1 ,&l1); // Create the space in the stack for comm_rank
	*stk(l1)=(double)comm_rank; // Copy comm_size into the stack

	LhsVar(1)= 1;
	C2F(putlhsvar)();
	return 0;
}

