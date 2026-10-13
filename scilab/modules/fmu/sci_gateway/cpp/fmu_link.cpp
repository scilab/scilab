//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2017-2020 - ESI Group - Clement David
// Copyright (C) - 2013-2017 - Scilab Enterprises - Clement David
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

#include <stdlib.h>
#include <string>

extern "C"
{
#include "sci_malloc.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_fmu.h"
}

#include "fmu_wrapper.hxx"

int fmu_link(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int *piAddressLib;
    int *piAddressPrefix;
    int *piAddressFmuImpl = nullptr;

    std::string varLib;
    std::string varPrefix;
    static const size_t VAR_IMPL_LEN = 6; // me 1.0
    std::string varImpl;
    FmuWrapper *instance;

    char *tmp = NULL;

    // The output argument is an instance pointer, a string type and a string version
    if (!checkOutputArgument(pvApiCtx, 0, 3))
    {
        return -1;
    }

    /*
     * Retrieve the common 2 arguments: library name, library prefix name
     */
    if (!checkInputArgument(pvApiCtx, 2, 4))
    {
        return -1;
    }
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressLib);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressPrefix);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return sciErr.iErr;
    }

    // optional argument, the FMU impl can be detected from the library
    int iRhs = *getNbInputArgument(pvApiCtx);
    if (2 < iRhs)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressFmuImpl);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
    }

    if (piAddressFmuImpl != nullptr && isStringType(pvApiCtx, piAddressFmuImpl))
    {
        if (getAllocatedSingleString(pvApiCtx, piAddressFmuImpl, &tmp))
        {
            return -1;
        }
        varImpl = tmp;
        FREE(tmp);
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressLib, &tmp))
    {
        return -1;
    }
    varLib = tmp;
    FREE(tmp);

    if (getAllocatedSingleString(pvApiCtx, piAddressPrefix, &tmp))
    {
        return -1;
    }
    varPrefix = tmp;
    FREE(tmp);

    if (piAddressFmuImpl == nullptr)
    {
        detect_fmu_version(varLib, varPrefix, varImpl);
        if (varImpl == "")
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: unable to detect FMU type.\n"), fname, 1);
            return 0;
        }
    }

    /*
     * allocate 
     */
    while (varImpl.size() >= VAR_IMPL_LEN)
    {
        if (varImpl.find("me 3.0") != std::string::npos)
        {
            instance = new Fmu3ModelExchange();
        }
        else if (varImpl.find("me 2.0") != std::string::npos)
        {
            instance = new Fmu2ModelExchange();
        }
        else if (varImpl.find("me 1.0") != std::string::npos)
        {
            instance = new FmuModelExchange();
        }
        else if (varImpl.find("cs 3.0")  != std::string::npos)
        {
            instance = new Fmu3CoSimulation();
        }
        else if (varImpl.find("cs 2.0") != std::string::npos)
        {
            instance = new Fmu2CoSimulation();
        }
        else if (varImpl.find("cs 1.0") != std::string::npos)
        {
            instance = new FmuCoSimulation();
        }
        else if (varImpl.find("se 3.0") != std::string::npos)
        {
            instance = new Fmu3ScheduledExecution();
        }
        else
        {
            int arg_error = 1;
            if (iRhs > 2)
            {
                arg_error = iRhs;
            }
            Scierror(999, _("%s: Wrong type for input argument #%d: FMU type expected. Got %s.\n"), fname, arg_error, varImpl.data());
            return 0;
        }

        if (instance->create(varLib, varPrefix) == false)
        {
            // something goes wrong, try again with another implementation
            delete instance;
            varImpl = varImpl.substr(VAR_IMPL_LEN);
        }
        else
        {
            // the instance has been correctly create with the first item in varImpl
            break;
        }
    }

    if (varImpl.size() < VAR_IMPL_LEN)
    {
        Scierror(999, _("%s: Unable to load %s.\n"), fname, varLib.data());
        return 0;
    }

    /*
     * Map the instance to a pointer scilab type
     */
    sciErr = createPointer(pvApiCtx, iRhs + 1, instance);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return sciErr.iErr;
    }
    AssignOutputVariable(pvApiCtx, 1) = nbInputArgument(pvApiCtx) + 1;

    int iLhs = *getNbOutputArgument(pvApiCtx);
    if (1 < iLhs)
    {
        std::string fmuImpl = varImpl.substr(0, VAR_IMPL_LEN);
        sciErr.iErr = createSingleString(pvApiCtx, iRhs + 2, fmuImpl.data());
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return sciErr.iErr;
        }
        AssignOutputVariable(pvApiCtx, 2) = nbInputArgument(pvApiCtx) + 2;
    }

    return 0;
}
