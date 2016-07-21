/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises
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

extern "C" {
#include "Scierror.h"
#include "gw_external_objects_java.h"
#include "sci_malloc.h"
}

#include "ScilabJavaEnvironment.hxx"

using namespace org_scilab_modules_external_objects_java;
using namespace org_modules_external_objects;

int sci_jcreatejar(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int res = 1;

    CheckInputArgument(pvApiCtx, 2, 4);
    CheckOutputArgument(pvApiCtx, 0, 1);

    // Input argument 1: jar file path
    int* addr1 = NULL;
    getVarAddressFromPosition(pvApiCtx, 1, &addr1);

    char *jarFilePath = NULL;
    if (getAllocatedSingleString(pvApiCtx, addr1, &jarFilePath))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 1);
        return 1;
    }

    // Input argument 2: paths of files to be jar-red
    int* addr2 = NULL;
    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &addr2);
    if (sciErr.iErr)
    {
        freeAllocatedSingleString(jarFilePath);
        printError(&sciErr, 0);
        return 1;
    }

    if (!isStringType(pvApiCtx, addr2))
    {
        freeAllocatedSingleString(jarFilePath);
        Scierror(202, _("%s: Wrong type for input argument #%d: String array expected.\n"), fname, 2);
        return 1;
    }

    int nbRow = 0;
    int nbCol = 0;
    sciErr = getVarDimension(pvApiCtx, addr2, &nbRow, &nbCol);
    if (sciErr.iErr)
    {
        freeAllocatedSingleString(jarFilePath);
        printError(&sciErr, 0);
        return 1;
    }

    if ((nbRow < 1 || nbCol != 1) && (nbCol < 1 || nbRow != 1))
    {
        freeAllocatedSingleString(jarFilePath);
        throw ScilabAbstractEnvironmentException(__LINE__, __FILE__, gettext("Invalid dimension for argument #%d: A row or a column expected."), 2);
    }
    int nbFilePaths = nbRow * nbCol;

    char **filePaths = NULL;
    if (getAllocatedMatrixOfString(pvApiCtx, addr2, &nbRow, &nbCol, &filePaths))
    {
        freeAllocatedSingleString(jarFilePath);
        printError(&sciErr, 0);
        return 1;
    }

    char *filesRootPath = NULL;
    if (nbInputArgument(pvApiCtx) > 2)
    {
        // Input argument 3: files root path
        int* addr3 = NULL;
        getVarAddressFromPosition(pvApiCtx, 3, &addr3);

        if (getAllocatedSingleString(pvApiCtx, addr3, &filesRootPath))
        {
            freeAllocatedSingleString(jarFilePath);
            freeAllocatedMatrixOfString(nbRow, nbCol, filePaths);
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 3);
            return 1;
        }
    }

    char *manifestFilePath = NULL;
    if (nbInputArgument(pvApiCtx) > 3)
    {
        // Input argument 4: manifest file path
        int* addr4 = NULL;
        getVarAddressFromPosition(pvApiCtx, 4, &addr4);

        if (getAllocatedSingleString(pvApiCtx, addr4, &manifestFilePath))
        {
            freeAllocatedSingleString(jarFilePath);
            freeAllocatedMatrixOfString(nbRow, nbCol, filePaths);
            freeAllocatedSingleString(filesRootPath);
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 4);
            return 1;
        }
    }

    try
    {
        ScilabJavaEnvironment::start();
        ScilabJavaEnvironment *javaEnvironment = ScilabJavaEnvironment::getInstance();
        if (javaEnvironment)
        {
            res = javaEnvironment->createJarArchive(jarFilePath, filePaths, nbFilePaths, filesRootPath, manifestFilePath);

            // Create boolean return value
            int *ret = NULL;
            sciErr = allocMatrixOfBoolean(pvApiCtx, nbInputArgument(pvApiCtx) + 1, 1, 1, &ret);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                freeAllocatedSingleString(jarFilePath);
                freeAllocatedMatrixOfString(nbRow, nbCol, filePaths);
                freeAllocatedSingleString(filesRootPath);
                freeAllocatedSingleString(manifestFilePath);
                return 1;
            }
            ret[0] = (res == 0) ? 1 : 0;
            AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;
            ReturnArguments(pvApiCtx);
        }
        else
        {
            Scierror(999, "%s: No Java environment available (instance is null).", fname);
        }
    }
    catch (std::exception &e)
    {
        Scierror(999, "%s: An error occured: %s", fname, e.what());
    }

    freeAllocatedSingleString(jarFilePath);
    freeAllocatedMatrixOfString(nbRow, nbCol, filePaths);
    freeAllocatedSingleString(filesRootPath);
    freeAllocatedSingleString(manifestFilePath);

    return 0;
}
