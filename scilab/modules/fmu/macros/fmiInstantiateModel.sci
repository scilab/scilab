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

//Creation and Destruction of Model Instances.

function instanceNew = fmiInstantiateModel(model, logger)
    // Check number of arguments
    if ~or(argn(2) == [1 2]) then
       error(msprintf(_("%s: Wrong number of input arguments: %d expected.\n"), "fmiInstantiateModel", 1, 2));
    end
    // Check type of arguments
    if typeof(model) <> "FMU" & typeof(model) <> "FMUinst" then
        error(999, msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiInstantiateModel", 1, "FMU"));
    elseif typeof(model) == "FMUinst" then
        error(999, msprintf(_("%s: Wrong type for input argument #%d: it is already an instance of FMU.\n"), "fmiInstantiateModel",1)); 
    end
    instanceNew = [];
    
    if ~exists("logger")
        logger = %t;
    else
        if typeof(logger) <> "boolean" then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiInstantiateModel", 2, "boolean"));
        end
        
        logger = logger(1);
    end
    // Call the function from library 
    instance = fmu_call(model.simulationLibrary, ...
        "fmiInstantiateModel", ...
        model.descriptionFMU.modelIdentifier, ...
        model.descriptionFMU.guid, ...
        logger);
        
    instanceNew = tlist(["FMUinst",...
        "instanceName",...
        "modelFMU",...
        "modelInstance",...
        "isInitialized"],...
        model.modelFileName, model, instance, %f);
endfunction

