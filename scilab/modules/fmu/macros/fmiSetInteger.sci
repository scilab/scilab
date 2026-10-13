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

//Set Integer values

function fmiSetInteger(model, refs, values)
    // Check number of arguments
    if argn(2) <> 3 then
       error(msprintf(_('%s: Wrong number of input arguments: %d expected.\n'),'fmiSetInteger',3));
    end
    // Check type of arguments
    if typeof(model) <> 'FMUinst' then 
        error(msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmiSetInteger', 1, 'FMU instance'));
    elseif typeof(refs) <> 'constant' then 
        error(msprintf(_('%s: Wrong type for input argument #%d: Real matrix expected.\n'), 'fmiSetInteger', 2));
    elseif typeof(values) <> 'constant' then 
        error(msprintf(_('%s: Wrong type for input argument #%d: Real matrix expected.\n'), 'fmiSetInteger', 3));
    end
    // Check length of vectors and value references
    if (length(refs)<>length(values)) then
        error(999, msprintf(_('%s: Incompatible input arguments #%d and #%d: Same sizes expected.\n'), 'fmiSetInteger',2,3));    
    elseif size(refs, "*")<>size(unique(refs), "*") then
        error(999, msprintf(_('%s: Wrong values for input argument #%d: Unique references expected.\n'), 'fmiSetInteger', 2));  
    end
    
    //check start values 
    startValuesAll = model.modelFMU.modelVariables.Integer.valueReference(find(~isnan(model.modelFMU.modelVariables.Integer.start)));
    maxsAll = model.modelFMU.modelVariables.Integer.max(find(~isnan(model.modelFMU.modelVariables.Integer.start)));
    minsAll = model.modelFMU.modelVariables.Integer.min(find(~isnan(model.modelFMU.modelVariables.Integer.start)));
    startValuesCurrent = startValuesAll(find(dsearch(startValuesAll, gsort(refs, "g","i"),"d") <> 0));
    if or(gsort(refs, "g","i") <> gsort(startValuesCurrent, "g","i")) then
       error(999, msprintf(_('%s: Wrong values for input argument #%d: References for start values expected.\n'), 'fmiSetInteger', 2));    
    end
    
    //check max and min values
    maxs = maxsAll(find(dsearch(startValuesAll, gsort(refs, "g","i"),"d") <> 0));
    mins = minsAll(find(dsearch(startValuesAll, gsort(refs, "g","i"),"d") <> 0));
    if or(values(:) > maxs(:)) then
       format('v',12);
       error(999, msprintf(_('%s: Wrong value for input argument #%d: Less expected for #%s valueReference(s).\n'),...
       'fmiSetInteger', 3, sci2exp(refs(find(values(:) > maxs(:))))));    
    elseif or(values(:) < mins(:)) then
       format('v',12);
       error(999, msprintf(_('%s: Wrong value for input argument #%d: More expected for #%s valueReference(s).\n'),...
       'fmiSetInteger', 3, sci2exp(refs(find(values(:) < mins(:))))));  
    end
    // negated alias 
    negatedAliasValues = model.modelFMU.modelVariables.Integer.valueReference(find(model.modelFMU.modelVariables.Integer.alias == "negatedAlias" &...
    ~isnan(model.modelFMU.modelVariables.Integer.start)));
    valuePosition = zeros(size(negatedAliasValues,"*"));
    for i=1:size(negatedAliasValues, "*")
        valuePosition(i) = find(refs == negatedAliasValues(i));
        values(valuePosition(i)) = -values(valuePosition(i));
    end
    // Call the function from library 
           status = fmu_call(model.modelFMU.simulationLibrary, 'fmiSetInteger', model.modelInstance, refs, values); 
           if status > 2 then //~fmiOk ~fmiWarning 
              fmiFreeModelInstance(model);
              error(msprintf(_('%s: The Integer values for %s have not been set.\n'), 'fmiSetInteger', model.instanceName));
           end
endfunction
