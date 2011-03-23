/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <mpi.h>
#include "stack-c.h"
#include "api_scilab.h"

#define __MPI_DEBUG__

typedef SciErr (*function_loadData)(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, void** _pdblReal);

typedef struct mappinpScilabMPI_struct{
	MPI_Datatype MPI;
	MPI_Datatype customMPI;
	sci_types Scilab;
	double *data; // TODO doit etre void
	int rows;  /* Number of elements sent */
	int cols;  /* Number of elements sent */
	function_loadData f;
} mappinpScilabMPI;
mappinpScilabMPI getMPIDataStructure(int position);

