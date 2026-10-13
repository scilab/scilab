//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

//Set debug logging 

function fmiSetDebugLogging(model, logging)
    // Check number of arguments
    if argn(2) <> 2 then
       error(msprintf(_('%s: Wrong number of input arguments: %d expected.\n'),'fmiSetDebugLogging',2));
    end
    // Check type of arguments
    if typeof(model) <> "FMUinst" then 
        error(999, msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmiSetDebugLogging', 1, 'FMU instance'));
    elseif typeof(logging) <> "boolean" | length(logging) <> 1 then
        error(999, msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmiSetDebugLogging', 2, 'boolean'));
    end
    // Call the function from library 
       status = fmu_call(model.modelFMU.simulationLibrary, 'fmiSetDebugLogging', model.modelInstance, logging);
       if status <> 0 then //fmiOk
          warning(msprintf(_('%s: It is not possible to set a debug logging for %s.\n'), 'fmiSetDebugLogging', model.instanceName)); 
       elseif status > 2 then
             fmiFreeModelInstance(model);
       end
endfunction
