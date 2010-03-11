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
#include "sci_mpi.h"

/******************************************
 * SCILAB function : mpi_finalize, fin = 2
 ******************************************/

int sci_mpi_finalize (char *fname,unsigned long fname_len)
{
 CheckRhs(0,0);
 CheckLhs(1,1);
 MPI_Finalize();
 // TODO: catch les erreurs
 LhsVar(1)= 0;
 C2F(putlhsvar)();

 return 0;

}
