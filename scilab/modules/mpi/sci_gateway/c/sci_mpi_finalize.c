/**
 * Function calling mpi_finalize
 * @author Sylvestre LEDRU INRIA
 * @date 2007
 */
#include "gw_mpi.h"
#include "sci_mpi.h"

/******************************************
 * SCILAB function : mpi_finalize, fin = 2
 ******************************************/

int C2F(sci_mpi_finalize) (char *fname,unsigned long fname_len)
{
 CheckRhs(0,0);
 CheckLhs(1,1);
 MPI_Finalize();
 // TODO: catch les erreurs
 LhsVar(1)= 0;
 C2F(putlhsvar)();

 return 0;

}
