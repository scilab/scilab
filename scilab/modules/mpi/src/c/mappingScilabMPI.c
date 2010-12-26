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
#include <mpi.h>
#include "api_scilab.h"
#include "mappingScilabMPI.h"
#include "Scierror.h"

static mappinpScilabMPI getMPIMapping(sci_types scilabType);
static MPI_Datatype setHomogenousScilabType(int size, MPI_Datatype dataType);
mappinpScilabMPI getMPIDataStructure(int position){
	int typevar;
    SciErr sciErr;
	mappinpScilabMPI mapping;
	int iRows, iCols;
	double *data;
	int *piAddr                     = NULL;
	printf("ici 0\n");
	sciErr = getVarAddressFromPosition(pvApiCtx, position, &piAddr);
	if(sciErr.iErr)
        {
			printError(&sciErr, 0);
        }
	sciErr = getVarType(pvApiCtx, piAddr, &typevar);
	if(sciErr.iErr)
        {
			printError(&sciErr, 0);
        }

	mapping=getMPIMapping((sci_types)typevar);

	switch (typevar){
		case sci_matrix:
			(*mapping.f)(pvApiCtx, piAddr, &iRows, &iCols, &data);
			//			getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &data);
			// TODO DATA devrait etre void ici
			printf("data: irows %d, icols %d\n",iRows, iCols);

			printf("data: %5.2f\n",(double)data[0]);
			printf("data: %5.2f\n",(double)data[1]);
			printf("sizeof(data): %d\n",iRows*iCols*sizeof(double));
			mapping.data=NULL;
			mapping.data=(double*)malloc((iRows * iCols + 2)*sizeof(double)); // Store iRows + iCols + the data
			
			double  plop = (double)iRows;
			double plip = (double)iCols;

			memcpy(mapping.data,&plop,sizeof(double));
			printf("VALUE : %5.2f\n", plop);
			printf("VALUE 2 : %5.2f\n", plip);
			printf("============recvValue 1 %5.2f ==========\n", mapping.data[0]);fflush(NULL);
			memcpy(mapping.data+1,&plip,sizeof(double));
			printf("============recvValue 2 %5.2f ==========\n", mapping.data[1]);fflush(NULL);

			memcpy(mapping.data+2,data,iRows*iCols*sizeof(double));
			printf("============recvValue 3 %5.2f ==========\n", mapping.data[2]);fflush(NULL);
			printf("============recvValue %5.2f ==========\n", mapping.data[3]);fflush(NULL);
			//printf("mapping data: %5.2f\n",(double)mapping.data[0]);
			mapping.rows=iRows;
			mapping.cols=iCols;
			printf("mapping.count %d\n",mapping.rows*mapping.cols);

            mapping.customMPI=setHomogenousScilabType(2+iRows*iCols, mapping.MPI); /* 2 for nb Rows & cols */
			break;
		case sci_strings:
			
			break;
		default:
			Scierror(999, "Datatype unknown\n");
			exit(1);
			break;
	}
	return mapping;
}

static mappinpScilabMPI getMPIMapping(sci_types scilabType){
	mappinpScilabMPI mapping;
	printf("scilabType getMPIMapping %d\n",scilabType);
	fflush(NULL);
	switch (scilabType){
		case sci_matrix:
			mapping.MPI=MPI_DOUBLE;
			printf("MPI plop %d \n",mapping.MPI);//, *data[0]);

			mapping.Scilab=scilabType;
			mapping.f=getMatrixOfDouble;
			/* @TODO: rajouter la gestion des pointeurs de fonction pour getmatrix */				
			break;
		case sci_strings:
			mapping.MPI=MPI_CHAR;
			mapping.Scilab=scilabType;
			//			mapping.f=getMatrixOfDouble;
			//			mapping.functionName=getMatrixOfDouble;
		
	}
	return mapping;

}


static MPI_Datatype setHomogenousScilabType(int size, MPI_Datatype dataType) {
	MPI_Datatype matrix;
	int ierr = MPI_Type_contiguous(size, dataType, &matrix);

	if (ierr != MPI_SUCCESS) 
		{
			fprintf(stderr,"an error occurred");
		}

	ierr = MPI_Type_commit(&matrix);
	if (ierr != MPI_SUCCESS) 
		{
			fprintf(stderr,"an error occurred");
		}
	return matrix;
}

/*
  sci_matrix = 1 ,
  sci_poly = 2 ,
  sci_boolean = 4 ,
  sci_sparse = 5  ,
  sci_boolean_sparse = 6,
  sci_matlab_sparse = 7, 
  sci_ints = 8,
  sci_handles =9,
  sci_strings = 10,
  sci_u_function = 11,
  sci_c_function = 13 ,
  sci_lib =  14,
  sci_list = 15,
  sci_tlist = 16,
  sci_mlist = 17,
  sci_pointer = 128, 
  sci_implicit_poly = 129,
  sci_intrinsic_function = 130
*/

