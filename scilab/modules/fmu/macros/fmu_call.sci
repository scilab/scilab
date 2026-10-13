//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2024 - Dassault Systèmes - Clément DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

function varargout = fmu_call(simulationLibrary, fname, varargin)
    global %_fmu_gateway_cache; // /!\ Do not rename this variable. Name is tracked/ignored by Variable Browser

    // Check number of arguments
    if argn(2) < 2 then
       error(msprintf(_('%s: Wrong number of input arguments: at least %d expected.\n'), 'fmu_call', 2));
    end
    // Check type of arguments
    if typeof(simulationLibrary) <> 'pointer' then 
        error(msprintf(_('%s: Wrong type for input argument #%d: %s expected.\n'), 'fmu_call', 1, 'pointer'));
    elseif typeof(fname) <> 'string' then 
        error(msprintf(_('%s: Wrong type for input argument #%d: String expected.\n'), 'fmu_call', 2));
    end
    
    // read the function table for the fname, parsed once then cached for the whole session
    if isempty(%_fmu_gateway_cache) then
        doc = xmlRead(fullfile(fmigetPath(), "src", "swig", "fmuswig_gateway.xml"));
        %_fmu_gateway_cache = struct("names", xmlAsText(xmlXPath(doc, "//gateway/@name")), "resolved", struct());
        xmlDelete(doc);
    end
    // if the fname is present on multiple mode (Model Exchange and CoSimulation), lets the fmu_wrapper resolve it through its inheritance chain
    if ~or(fieldnames(%_fmu_gateway_cache.resolved) == fname) then
        matches = grep(%_fmu_gateway_cache.names, fname);
        %_fmu_gateway_cache.resolved(fname) = %_fmu_gateway_cache.names(matches(1));
    end
    resolvedGateway = %_fmu_gateway_cache.resolved(fname);

    // Call the function from library
    vargsin = "";
    for i=1:length(varargin)
        vargsin = [vargsin ", varargin("+string(i)+")"];
    end
    vargsout = "";
    lhs = argn(1);
    if lhs > 0 then
        vargsout = "varargout(1)";
    end
    for i=2:lhs
        vargsout = [vargsout ", varargout("+string(i)+")"];
    end
    if lhs > 0 then
      vargsout = ["[" vargsout "] = "];
    end
    
    execstr(strcat(vargsout) + resolvedGateway + "(simulationLibrary" + strcat(vargsin) + ")");
    
    // recreate struct() if needed
    if  lhs > 1 & or(fname == ["fmiInitialize", "fmiEventUpdate"]) then
        event = varargout(2);
        // event is a double vector, eventInfo is a struct
        eventInfo = struct("iterationConverged", event(1) <> 0, ..
             "stateValueReferencesChanged"     , event(2) <> 0, ..
             "stateValuesChanged"              , event(3) <> 0, ..
             "terminateSimulation"             , event(4) <> 0, ..
             "upcomingTimeEvent"               , event(5) <> 0, ..
             "nextEventTime"                   , event(6));
        varargout(2) = eventInfo;
    end
endfunction
