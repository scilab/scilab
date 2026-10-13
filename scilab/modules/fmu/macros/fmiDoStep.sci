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

function status = fmiDoStep(model, current, step, newstep)
    if ~or(argn(2) == [3, 4]) then
       error(msprintf(_("%s: Wrong number of input arguments: %d or %d expected.\n"),"fmiDoStep", 3, 4));
    end
    if ~exists("newstep") then
        newstep = %t;
    end
    
    if typeof(model) <> "FMUinst" then 
        error(999, msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiDoStep", 1, "FMU instance"));
    end
    
    if typeof(current) <> "constant" | length(current) <> 1 then 
        error(999, msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiDoStep", 2, "real"));
    end
    
    if typeof(step) <> "constant" | length(step) <> 1 then 
        error(999, msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiDoStep", 3, "real"));
    end
    
    if typeof(newstep) <> "boolean" | length(newstep) <> 1 then 
        error(999, msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiDoStep", 4, "boolean"));
    end
    
    
    // Call the function from library 
    if ~model.isInitialized then 
        error(msprintf(_("%s: Model %s is not initialized.\n"), "fmiDoStep", model.instanceName));
    end
    status = fmu_call(model.modelFMU.simulationLibrary,"fmiDoStep", model.modelInstance, current, step, newstep);
endfunction
