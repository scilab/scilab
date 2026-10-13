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

//Get derivatives states 

function getStates = fmiGetDerivatives(model)
    // Check number of arguments
    if argn(2) <> 1 then
       error(msprintf(_('%s: Wrong number of input arguments: %d expected.\n'),'fmiGetDerivatives',1));
    end
    // Check type of arguments
    if typeof(model) <> 'FMUinst' then 
        error(999, msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmiGetDerivatives', 1, 'FMU instance'));
    end
    // Call the function from library 
           getStates = zeros(model.modelFMU.descriptionFMU.numberOfContinuousStates);
           [status, states] = fmu_call(model.modelFMU.simulationLibrary, 'fmiGetDerivatives', model.modelInstance, model.modelFMU.descriptionFMU.numberOfContinuousStates); 
           if status > 2 then //~fmiOk ~fmiWarning  
              fmiFreeModelInstance(model);
              error(msprintf(_('%s: Derivatives states for %s have not been received.\n'), 'fmiGetDerivatives', model.instanceName));
           end
      getStates = states;
endfunction
