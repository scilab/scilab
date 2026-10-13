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

function eventInfo = fmiEventUpdate(model, intermediateResults)
    // Check number of arguments
    if argn(2) <> 2 then
       error(msprintf(_('%s: Wrong number of input arguments: %d expected.\n'),'fmiEventUpdate',2));
    end
    // Check type of arguments
    if typeof(model) <> 'FMUinst' then 
        error(999, msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmiEventUpdate', 1, 'FMU instance'));
    elseif typeof(intermediateResults) <> 'boolean' | length(intermediateResults) <> 1 then 
        error(999, msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmiEventUpdate', 2, 'boolean'));
    end
       // Call the function from library
       [status, eventInfo] = fmu_call(model.modelFMU.simulationLibrary,'fmiEventUpdate', model.modelInstance, intermediateResults);
       if status > 2 then //~fmiOk ~fmiWarning 
          fmiFreeModelInstance(model);
          error(msprintf(_('%s: Information about the events for %s has not been received.\n'), 'fmiEventUpdate', model.instanceName));
       end
endfunction
