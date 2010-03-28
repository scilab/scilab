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
#include <mpi.h>
#include "api_scilab.h"
#include "mappingScilabMPI.h"


static mappinpScilabMPI getMPIMapping(sci_types scilabType);

mappinpScilabMPI getMPIDataStructure(int position){
	int typevar;
    SciErr sciErr;
	mappinpScilabMPI mapping;
	int iRows, iCols;
	void *data;
	int *piAddr                     = NULL;
	printf("ici 0\n");
	sciErr = getVarAddressFromPosition(pvApiCtx, position, &piAddr);
	if(sciErr.iErr)
        {
			printError(&sciErr, 0);
        }
	printf("ici 1\n");
	sciErr = getVarType(pvApiCtx, piAddr, &typevar);
	if(sciErr.iErr)
        {
                printError(&sciErr, 0);
        }
	printf("ici 2\n");
	mapping=getMPIMapping((sci_types)typevar);
	printf("ici 3\n");
	switch (typevar){
		case sci_matrix:
			(*mapping.f)(pvApiCtx, piAddr, &iRows, &iCols, &data);
//			getMatrixOfDouble(pvApiCtx, piAddr, &iRows, &iCols, &data);
			printf("ici 4\n");
			mapping.data=data;
			mapping.count=iRows*iCols;
			printf("count %d \n",mapping.count);//, *data[0]);
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

