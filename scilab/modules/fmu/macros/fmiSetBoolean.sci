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

//Set Boolean values

function fmiSetBoolean(model, refs, values)
    // Check number of arguments
    if argn(2) <> 3 then
       error(msprintf(_('%s: Wrong number of input arguments: %d expected.\n'),'fmiSetBoolean',3));
    end
    // Check type of arguments
    if typeof(model) <> 'FMUinst' then 
        error(msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmiSetBoolean', 1, 'FMU instance'));
    elseif typeof(refs) <> 'constant' then 
        error(msprintf(_('%s: Wrong type for input argument #%d: Real matrix expected.\n'), 'fmiSetBoolean', 2));
    elseif typeof(values) <> 'boolean' then 
        error(msprintf(_('%s: Wrong type for input argument #%d: Boolean matrix expected.\n'), 'fmiSetBoolean', 3));
    end 
    // Check the length of vectors and value references
    if (length(refs)<>length(values)) then
        error(999, msprintf(_('%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n'), 'fmiSetBoolean',2,3));    
    elseif size(refs, "*")<>size(unique(refs), "*") then
        error(999, msprintf(_('%s: Wrong values for input argument #%d: Unique references expected.\n'), 'fmiSetBoolean', 2));  
    end
    
    //check start values // 'outputs' can not be set, it doesn't check here 
    startValuesAll = model.modelFMU.modelVariables.Boolean.valueReference(find(model.modelFMU.modelVariables.Boolean.variability == "discrete" &...
    model.modelFMU.modelVariables.Boolean.alias == "noAlias"));
    startValuesCurrent = startValuesAll(find(dsearch(startValuesAll, gsort(refs, "g","i"),"d") <> 0));
    if or(gsort(refs, "g","i") <> gsort(startValuesCurrent, "g","i")) then
       error(999, msprintf(_('%s: Wrong values for input argument #%d: References for start values expected.\n'), 'fmiSetBoolean', 2));    
    end
    // negated alias 
    negatedAliasValues = model.modelFMU.modelVariables.Boolean.valueReference(find(model.modelFMU.modelVariables.Boolean.alias == "negatedAlias" &...
    model.modelFMU.modelVariables.Boolean.variability == "discrete"));
    valuePosition = zeros(size(negatedAliasValues,"*"));
    for i=1:size(negatedAliasValues, "*")
        valuePosition(i) = find(refs == negatedAliasValues(i));
        values(valuePosition(i)) = ~values(valuePosition(i));
    end
    // Call the function from library 
           status = fmu_call(model.modelFMU.simulationLibrary, 'fmiSetBoolean', model.modelInstance, refs, values); 
           if status > 2 then // ~fmiOk ~fmiWarning
              fmiFreeModelInstance(model);
              error(msprintf(_('%s: The Boolean values for %s have not been set.\n'), 'fmiSetBoolean', model.instanceName));
           end
endfunction
