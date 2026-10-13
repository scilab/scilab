function instanceNew = fmiInstantiateSlave(model, timeout, logger)
    // Check number of arguments
    if argn(2) <> [1:3] then
       error(msprintf(_("%s: Wrong number of input arguments: %d to %d expected.\n"), "fmiInstantiateSlave", 1, 3));
    end
    // Check type of arguments
    if typeof(model) <> "FMU" & typeof(model) <> "FMUinst" then
        error(999, msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiInstantiateSlave", 1, "FMU"));
    elseif typeof(model) == "FMUinst" then
        error(999, msprintf(_("%s: Wrong type for input argument #%d: it is already an instance of FMU.\n"), "fmiInstantiateSlave",1)); 
    end
    
    //check timeout
    if ~exists("timeout") then
        timeout = 5;
    end
        
    if ~exists("logger")
        logger = %t;
    else
        if typeof(logger) <> "boolean" then
            error(999, msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiInstantiateSlave", 3, "boolean"));
        end
        
        logger = logger(1);
    end

    instanceNew = [];
    // Call the function from library ; no fmuLocation, visible and interactive are false
    instance = fmu_call(...
        model.simulationLibrary, "fmiInstantiateSlave", ...
        model.descriptionFMU.modelIdentifier, ...
        model.descriptionFMU.guid, ...
        "", ...
        "application/x-fmu-sharedlibrary", ...
        timeout, ...
        %f, ...
        %f, ...
        logger);
    
    instanceNew = tlist(["FMUinst",...
         "instanceName",...
         "modelFMU",...
         "modelInstance",...
         "isInitialized"],...
          model.modelFileName, model, instance, %f);
endfunction

