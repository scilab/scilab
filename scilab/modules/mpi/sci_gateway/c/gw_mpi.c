/**
 * Declaration of the "linking" functions Scilab <=> native code 
 * (here C & Fortran)
 * @author Sylvestre LEDRU INRIA 
 * @date 2007
 */
#include "gw_mpi.h"
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
static MPITable Tab[]=
{
	{C2F(sci_mpi_init),"MPI_Init"},
	{C2F(sci_mpi_finalize),"MPI_Finalize"},
	{C2F(sci_mpi_comm_size),"MPI_Comm_size"},
	{C2F(sci_mpi_comm_rank),"MPI_Comm_rank"},
	{C2F(sci_mpi_recv),"MPI_Recv"},
	{C2F(sci_mpi_send),"MPI_Send"}
};
/*static MPITable Tab[]=
{
	{intsmpi_send,"mpi_send"},
	{intsmpi_recv,"mpi_recv"},
	{intsmpi_isend,"mpi_isend"},
	{intsmpi_irecv,"mpi_irecv"},
	{intsmpi_init,"mpi_init"},
	{intsmpi_finalize,"mpi_finalize"},
	{intsmpi_comm_rank,"mpi_comm_rank"},
	{intsmpi_comm_size,"mpi_comm_size"}
};*/
/*-----------------------------------------------------------------------------------*/
int C2F(gw_mpi)(void)
{  
	Rhs = Max(0, Rhs);

#ifdef _MSC_VER
	#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{	
			ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
		}
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	#endif
#else
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
