/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2011 - DIGITEO - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include "gw_mpi.h"
#include "sci_mpi.h"
#include "mappingScilabMPI.h"

int sci_mpi_serialize (char *fname, unsigned long fname_len)
{
	int err;
	CheckRhs(2,2);
	CheckLhs(0,1);
	mappinpScilabMPI sciData = getMPIDataStructure(2);
    FILE *pFile = fopen( "/tmp/serialisation.mpi", "wb" );
    err=fwrite (&sciData.rows , 1 , sizeof(int),  pFile );
    err=fwrite (&sciData.cols , 1 , sizeof(int),  pFile );
    err=fwrite (&sciData.Scilab , 1 , sizeof(sci_types), pFile );
	printf("nb element: %d\n",sciData.rows*sciData.cols);
    err=fwrite (&sciData.data , sizeof(double), sciData.rows*sciData.cols, pFile );
	printf("err %d\n",err);
    fclose(pFile);

	LhsVar(1)= 0;
	C2F(putlhsvar)();
	return 0;
}
