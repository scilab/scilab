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

//Get Real values
function realValues = fmiGetReal(model, refs)
    // Check number of arguments
    if argn(2) <> 2 then
       error(msprintf(_('%s: Wrong number of input arguments: %d expected.\n'),'fmiGetReal',2));
    end
    // Check type of arguments
    if typeof(model) <> 'FMUinst' then 
        error(msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmiGetReal', 1, 'FMU instance'));
    elseif typeof(refs) <> 'constant' then 
        error(msprintf(_('%s: Wrong type for input argument #%d: Real matrix expected.\n'), 'fmiGetReal', 2));
    end 
    // Call the function from library 
           realValues = zeros(size(refs, "*"));
           [status, values] = fmu_call(model.modelFMU.simulationLibrary, 'fmiGetReal', model.modelInstance, refs); 
           if status > 2 then //~fmiOk ~fmiWarning
              fmiFreeModelInstance(model);
              error(msprintf(_('%s: The Real values for %s have not been received.\n'), 'fmiGetReal', model.instanceName));
           end
           // negated alias
           negatedAliasValues = model.modelFMU.modelVariables.Real.valueReference(find(model.modelFMU.modelVariables.Real.alias == "negatedAlias" &...
           ~isnan(model.modelFMU.modelVariables.Real.start)));
           valuePosition = zeros(size(negatedAliasValues,"*"));
           for i=1:size(negatedAliasValues, "*")
               valuePosition(i) = find(refs == negatedAliasValues(i));
               values(valuePosition(i)) = -values(valuePosition(i));
           end
      realValues = values;
endfunction
