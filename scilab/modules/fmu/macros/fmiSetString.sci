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

//Set string values

function fmiSetString(model, refs, values)
    // Check number of arguments
    if argn(2) <> 3 then
       error(msprintf(_('%s: Wrong number of input arguments: %d expected.\n'),'fmiSetString',3));
    end
    // Check type of arguments
    if typeof(model) <> 'FMUinst' then 
        error(999, msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmiSetString', 1, 'FMU instance'));
    elseif typeof(refs) <> 'constant' then 
        error(999, msprintf(_('%s: Wrong type for input argument #%d: Real matrix expected.\n'), 'fmiSetString', 2));
    elseif typeof(values) <> 'string' then 
        error(999, msprintf(_('%s: Wrong type for input argument #%d: Matrix of strings expected.\n'), 'fmiSetString', 3));
    end
    // Check the length of vectors
    if (length(refs)<>size(values, "*")) then
       error(999, msprintf(_('%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n'), 'fmiSetString',2,3));
    end
    // Call the function from library 
          status = fmu_call(model.modelFMU.simulationLibrary, 'fmiSetString', model.modelInstance, refs, values); 
          if status > 2 then //~fmiOk ~fmiWarning
             fmiFreeModelInstance(model);
             error(10000, msprintf(_('%s: The String values for %s have not been set.\n'), 'fmiSetString', model.instanceName));
          end
endfunction
