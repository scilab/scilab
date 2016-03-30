/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - S/E - Sylvestre Ledru
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <stdio.h>
#include <mpi.h>                        // for MPI_Error_string, MPI_Init, etc
#include <stddef.h>                     // for NULL
#include "localization.h"               // for _
#include "initMPI.h"

MPI_Errhandler errhdl;

void MPIErrHandler(MPI_Comm * comm, int *errorcode, ...)
{
    char buffer[4096];
    int resultlen;

    MPI_Error_string(*errorcode, buffer, &resultlen);
    buffer[resultlen] = 0;
    fprintf(stderr, _("MPI error: %s\n"), buffer);
}


int initScilabMPI()
{
    int flag;
    MPI_Initialized(&flag);
    if (!flag)
    {
        /* MPI Not yet initialized */
        MPI_Init(NULL, NULL);
    }
    return 0;
}
