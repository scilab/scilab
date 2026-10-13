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

function [eventInfo, instance] = fmiInitialize(model, toleranceControlled, relativeTolerance)
    // Check number of arguments
    if argn(2) <> 3 then
       error(msprintf(_('%s: Wrong number of input arguments: %d expected.\n'),'fmiInitialize',3));
    end
    // Check type of arguments
    if typeof(model) <> 'FMUinst' then 
        error(999, msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmiInitialize', 1, 'FMU instance'));
    elseif typeof(toleranceControlled) <> 'boolean' | length(toleranceControlled) <> 1 then 
        error(999, msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmiInitialize', 2, 'boolean'));
    elseif typeof(relativeTolerance) <> 'constant' | length(relativeTolerance) <> 1 then 
        error(999, msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmiInitialize', 3, 'scalar'));
    end
    // Call the function from library 
    if model.isInitialized then 
       warning(msprintf(_('%s: Model %s is already initialized.\n'), 'fmiInitialize', model.instanceName));
    else
       [status, eventInfo] = fmu_call(model.modelFMU.simulationLibrary,'fmiInitialize', model.modelInstance, toleranceControlled, relativeTolerance);
       if status > 2 then //~fmiOk ~fmiWarning 
          fmiFreeModelInstance(model);
          error(msprintf(_('%s: Model %s has not been initialized.\n'), 'fmiInitialize', model.instanceName));
       else
          model.isInitialized = %t;
          instance = model;
       end
    end
endfunction
