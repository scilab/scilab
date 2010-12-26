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
#include <stdio.h>
#include "api_scilab.h"
#include "gw_mpi.h"
#include "sci_mpi.h"

int sci_mpi_unserialize (char *fname, unsigned long fname_len)
{
	int err;
	CheckRhs(1,1);
	CheckLhs(0,1);

    FILE *pFile = fopen ( "/tmp/serialisation.mpi" , "rb" );
    if (pFile==NULL) {sprintf(stderr,"error while reading"); exit (1);}
    int rows, cols;
//    sci_types Scilab;
    char Scilab;
    double *data;

    err=fread(&rows, sizeof(int), 1, pFile);
    printf("rows: %d\n",rows);

    err=fread(&cols, sizeof(int), 1, pFile);
    printf("cols: %d\n",cols);

    err=fread(&Scilab, sizeof(sci_types), 1, pFile);
    
    printf("type: %d\n",Scilab);
    data = (double*)malloc(rows*cols*sizeof(double));
    err=fread(&data, sizeof(double), rows*cols, pFile);
    printf("data: %5.2f\n",data[0]);
	printf("err %d\n",err);
	createMatrixOfDouble(pvApiCtx, Rhs + 1, rows, cols, data);	
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();
	/*	
    for (i=0; i<rows*cols; i++) {
        printf("data[%d]: %f\n",i, data[i]);
		}*/
    fclose(pFile);


	return 0;
}
