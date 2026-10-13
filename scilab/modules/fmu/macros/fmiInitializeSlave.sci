//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2018 - ESI - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function instance = fmiInitializeSlave(model, startTime, stopTime)
    if argn(2) <> [2, 3] then
       error(msprintf(_("%s: Wrong number of input arguments: %d or %d expected.\n"),"fmiInitializeSlave", 2, 3));
    end
    
    if typeof(model) <> "FMUinst" then 
        error(999, msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiInitializeSlave", 1, "FMU instance"));
    end
    
    if typeof(startTime) <> "constant" | length(startTime) <> 1 then 
        error(999, msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiInitializeSlave", 2, "real"));
    end
    
    // Call the function from library 
    if model.isInitialized then 
        warning(msprintf(_("%s: Model %s is already initialized.\n"), "fmiInitializeSlave", model.instanceName));
    else
        if exists("stopTime") then
            status = fmu_call(model.modelFMU.simulationLibrary,"fmiInitializeSlave", model.modelInstance, startTime, %t, stopTime);
        else
            status = fmu_call(model.modelFMU.simulationLibrary,"fmiInitializeSlave", model.modelInstance, startTime, %f, 0);
        end
        
        if status > 2 then //~fmiOk ~fmiWarning 
            fmiFreeSlaveInstance(model);
            error(msprintf(_("%s: Model %s has not been initialized.\n"), "fmiInitializeSlave", model.instanceName));
        end
        model.isInitialized = %t;
    end

    instance = model;
endfunction
