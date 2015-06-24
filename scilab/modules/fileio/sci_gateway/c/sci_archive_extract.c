/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Kartik Gupta
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <stdlib.h>
#include <string.h>
#include "gw_fileio.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "PATH_MAX.h"
#include "archive_extract.h"
#include "freeArrayOfString.h"
#include "expandPathVariable.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include <unistd.h>
#include "isdir.h"
#include "createdirectory.h"
#include "scicurdir.h"
/*--------------------------------------------------------------------------*/
int sci_archive_extract(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    BOOL perm_flag = FALSE; /* default */

    int *piAddressVarOne = NULL;
    char **pStVarOne = NULL;
    char *pathextended = NULL;
    char **pStVarThree = NULL;
    char *pathextended_destination = NULL;
    char *current_directory;
    int error;
    int m1 = 0;
    int n1 = 0;

    int result;
    /* Check Input & Output parameters */
    CheckRhs(1, 3);
    CheckLhs(1, 1);
    if (Rhs > 2)
    {
        int *piAddressVarThree = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }
        if (!isStringType(pvApiCtx, piAddressVarThree))
        {
            if (isEmptyMatrix(pvApiCtx, piAddressVarThree))
            {
                createEmptyMatrix(pvApiCtx, Rhs + 1);
                LhsVar(1) = Rhs + 1;
                PutLhsVar()
            }
            else
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
            }
            return 0;
        }
        if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarThree, &m1, &n1, &pStVarThree) != 0)
        {
            freeAllocatedMatrixOfString(m1, n1, pStVarThree);
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

    }
    if (Rhs > 1)
    {
        int *piAddressVarTwo = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (isBooleanType(pvApiCtx, piAddressVarTwo) == 0 || isScalar(pvApiCtx, piAddressVarTwo) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, 2);
            return 0;
        }

        if (getScalarBoolean(pvApiCtx, piAddressVarTwo, &perm_flag))
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }
    }
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (!isStringType(pvApiCtx, piAddressVarOne))
    {
        if (isEmptyMatrix(pvApiCtx, piAddressVarOne))
        {
            createEmptyMatrix(pvApiCtx, Rhs + 1);
            LhsVar(1) = Rhs + 1;
            PutLhsVar()
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
        }
        return 0;
    }
    if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &pStVarOne) != 0)
    {
        freeAllocatedMatrixOfString(m1, n1, pStVarOne);
        Scierror(999, _("%s: No more memory.\n"), fname);
        return 0;
    }

    pathextended = expandPathVariable(pStVarOne[0]);
    if (pStVarThree != NULL)
    {
        char pathextended_temp[1024];
        pathextended_temp[0] = '\0';
        current_directory = scigetcwd(&error);
        if (error != 0)
        {
            Scierror(999, _("%s: Sorry the current working directory could not be found.\n"), fname);
            return 0;
        }
        error = 0;
#ifdef _WIN32
        if (pathextended[1] != ':' || pathextended[2] != '\')       //Checking if the path is absolute or relative for windows
                {
                strcat(pathextended_temp,current_directory);
                strcat(pathextended_temp,'\');
                strcat(pathextended_temp,pathextended);
                pathextended = pathextended_temp;
            }
#else
        if(!(pathextended[0] == ' / '))          //Checking if the path is absolute or relative for linux
        {
            strcat(pathextended_temp,current_directory);
            strcat(pathextended_temp,"/");
            strcat(pathextended_temp,pathextended);
            pathextended = pathextended_temp;
        }
#endif
                pathextended_destination = expandPathVariable(pStVarThree[0]);
                if(!isdir(pathextended_destination))
                {
                if(!createdirectory(pathextended_destination))
                {
                Scierror(999, _("%s: Sorry the destinations folder could not be created.\n"), fname);
                return 0;
            }
            }
                if(chdir(pathextended_destination)!=0)
                {
                Scierror(999, _("%s: Sorry the destination folder could not be opened.\n"), fname);
                return 0;
            }

            }
                if(perm_flag == FALSE)
                {
                result = archive_extract(pathextended,0,&error);
            }
                else if(perm_flag == TRUE)
                {
                result = archive_extract(pathextended,1,&error);
            }
                if(error == 1)
                {
                Scierror(999, _("%s: Sorry the archive could not be opened.\n"), fname);
                return 0;
            }
                if(error == 2)
                {
                Scierror(999, _("%s: Sorry the archive header could not be read.\n"), fname);
                return 0;
            }
                if(error == 3)
                {
                Scierror(999, _("%s: Sorry the archive header could not be written.\n"), fname);
                return 0;
            }
                if(error == 4)
                {
                Scierror(999, _("%s: Sorry the file data could not be read.\n"), fname);
                return 0;
            }
                if(error == 5)
                {
                Scierror(999, _("%s: Sorry the file data could not be written.\n"), fname);
                return 0;
            }
                if(error == 6)
                {
                Scierror(999, _("%s: Sorry the archive header could not be closed.\n"), fname);
                return 0;
            }


                if(pStVarThree!=NULL)
                {
                if(chdir(current_directory)!=0)
                {
                Scierror(999, _("%s: Sorry could not return to the current directory.\n"), fname);
                return 0;
            }
                freeAllocatedMatrixOfString(m1, n1, pStVarThree);
            }

                freeAllocatedMatrixOfString(m1, n1, pStVarOne);
                createScalarInteger32(pvApiCtx, Rhs + 1, result);
                LhsVar(1) = Rhs + 1;
                PutLhsVar();
                return 0;

            }
                /*--------------------------------------------------------------------------*/


