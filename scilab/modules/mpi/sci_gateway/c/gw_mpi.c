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
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "machine.h"
/*-----------------------------------------------------------------------------------*/
static MPITable Tab[]=
{
	{sci_mpi_init,"MPI_Init"},
	{sci_mpi_finalize,"MPI_Finalize"},
	{sci_mpi_comm_size,"MPI_Comm_size"},
	{sci_mpi_comm_rank,"MPI_Comm_rank"},
	{sci_mpi_recv,"MPI_Recv"},
	{sci_mpi_send,"MPI_Send"},
	{sci_mpi_serialize,"MPI_serialize"},
	{sci_mpi_unserialize,"MPI_unserialize"}
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
int gw_mpi(void)
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
