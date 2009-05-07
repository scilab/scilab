/**
 * Function calling mpi_init
 * @author Sylvestre LEDRU INRIA
 * @date 2007
 */
#include "gw_mpi.h"
#include "sci_mpi.h"

/**
 * SCILAB function : mpi_init, fin = 1
 * This function init the mpi env
 */
MPI_Errhandler errhdl;
/* should be moved elsewhere */
void MPIErrHandler(MPI_Comm *comm, int *errorcode, ...) {
  char buffer[4096];
  int resultlen;
  MPI_Error_string(*errorcode,buffer,&resultlen);
  buffer[resultlen] = 0;
  printf("Erreur mpi : %s\n",buffer);
}

int C2F(sci_mpi_init) (char *fname,unsigned long fname_len)
{
	int flag;
	CheckRhs(0,0);
	CheckLhs(1,1);

	MPI_Initialized(&flag);
	if (!flag) {
		/* MPI Not yet initialized */
		MPI_Init(NULL,NULL);

		MPI_Comm_create_errhandler(MPIErrHandler,&errhdl);
		//		MPI_Comm_set_errhandler(MPI_COMM_WORLD, errhdl);

		printf("MPI_INIT : init done\n");
	}
	LhsVar(1)= 0;
	C2F(putlhsvar)();
	return 0;
}
