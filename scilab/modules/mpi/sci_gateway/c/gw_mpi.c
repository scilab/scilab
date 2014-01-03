/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2009 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "gw_mpi.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "callFunctionFromGateway.h"
/*-----------------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
    {sci_mpi_init, "MPI_Init"},
    {sci_mpi_finalize, "MPI_Finalize"},
    {sci_mpi_comm_size, "MPI_Comm_size"},
    {sci_mpi_comm_rank, "MPI_Comm_rank"},
    {sci_mpi_recv, "MPI_Recv"},
    {sci_mpi_send, "MPI_Send"},
    {sci_mpi_get_processor_name, "MPI_Get_processor_name"},
    {sci_mpi_bcast, "MPI_Bcast"},
    {sci_mpi_irecv, "MPI_Irecv"},
    {sci_mpi_isend, "MPI_Isend"},
    {sci_mpi_wait, "MPI_Wait"}

};
/*-----------------------------------------------------------------------------------*/
int gw_mpi(void)
{
    Rhs = Max(0, Rhs);

    if (pvApiCtx == NULL)
    {
        pvApiCtx = (StrCtx*)MALLOC(sizeof(StrCtx));
    }

    pvApiCtx->pstName = (char*)Tab[Fin - 1].name;
    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}

/*-----------------------------------------------------------------------------------*/
