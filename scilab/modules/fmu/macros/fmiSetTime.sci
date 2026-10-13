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

//Set time in FMU 

function fmiSetTime(model, time)
    // Check number of arguments
    if argn(2) <> 2 then
       error(msprintf(_('%s: Wrong number of input arguments: %d expected.\n'),'fmiSetTime',2));
    end
    // Check type of arguments
    if typeof(model) <> 'FMUinst' then 
        error(999, msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmiSetTime', 1, 'FMU instance'));
    elseif typeof(time) <> "constant" | length(time) <> 1 then
        error(999, msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmiSetTime', 2, 'scalar'));
    end
    // Call the function from library 
           status = fmu_call(model.modelFMU.simulationLibrary, 'fmiSetTime', model.modelInstance, time);
           if status > 2 then //~fmiOk ~fmiWarning  
              fmiFreeModelInstance(model);
              error(msprintf(_('%s: Time for %s has not been set.\n'), 'fmiSetTime', model.instanceName));
           end
endfunction
