//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Clement David
// Copyright (C) - 2013 - Scilab Enterprises - Vladislav Trubkin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// Simulation of a model with default values or with user values for all variables.
// fmu = a model, can be retrieved with "importFMU"
// simTime = a vector: [start of simulation,sampling period,end of simulation]
// logger = debug logging(printing the messages in the Scilab console)
// Configuration the values of variables via the structures 'setValues' and 'getValues' in a model
// (e.g. fmu.setValues, fmu can be retrieved with the function "importFMU")

// out = fmiSimulate(model) simulation with default values of the time and the debug logging
// out = fmiSimulate(model,[0,0.001,100],%t) start time = 0, dtime = 0.001, end time = 100, %t debug logging 'On'

// User values for simulation:
// fmu = importFMU("path for a model");
// fmu.setValues it is the structure to set the values for a simulation
// fmu.getValues it is the structure to get the values of a siumulation
// Set Real values:
// All names to set values must be the names of real parameters, variables
// fmu.setValues.realNames = ["name1","name2","name3"]
// fmu.setValues.realValues = [1,2,3]
// number 1 corresponds of the "name1"; number 2 corresponds of the "name2" ....
// Set Integer values:
// All names to set values must be the names of integer parameters, variables
// fmu.setValues.integerNames = ["name1","name2","name3"]
// fmu.setValues.integerValues = [1,2,3]
// number 1 corresponds of the "name1"; number 2 corresponds of the "name2" ....
// Set Boolean values:
// All names to set values must be the names of boolean parameters, variables
// fmu.setValues.booleanNames = ["name1","name2","name3"]
// fmu.setValues.booleanValues = [%t,%f,%t]
// %t corresponds of the "name1"; %f corresponds of the "name2" ....
// Set String values
// at the moment no any effect

// get Values of the simulation
// Get Real values
// fmu.getValues.realNames = ["name1","name2","name3"]
// Get Integer values
// fmu.getValues.integerNames = ["name1","name2","name3"]
// Get Boolean values
// fmu.getValues.booleanNames = ["name1","name2","name3"]
// Get String values
// not yet supported
// Names must have the same types that the name of fields
// (e.g. fmu.getValues.realNames = ["name1","name2","name3"] where name1, name2, name3 are the names of real parameters, variables, constants)

// set Inputs values
// It is possible to set the inputs as a functions of the time
// Loading the names of inputs in the structure has been done at the moment of the import of FMU in Scilab(function importFMU)
// if fmu.setValues.inputs = [] the model have not any inputs.
// else fmu.setValues.inputs = ["input1","input2","input3"...]
// All types of inputs are in the same vector.
// For every input we have the fields to set the values: fmu.setValues.input_1_values for "input1", fmu.setValues.input_2_values for "input2" ...
// fmu.setValues.input_1_values must be a vector with two rows where the first row is the values for a variable,
// the second row is the values of the time. Two rows must have the same size.

// If no changes were made in the structure for set/getValues the simulation will done with default values for all variables, parameters.

function out = fmiSimulate(fmu, simTime, logger)
    // Check number of arguments
    if argn(2) <> [1,3] then
       error(msprintf(_("%s: Wrong number of input arguments: %d or %d expected.\n"), "fmiSimulate",1,3));
    end
    if argn(2) == 1 then
       if typeof(fmu) <> "FMU" then
          error(msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiSimulate", 1, "FMU"));
       end
            // retrieve the start time from XML description
            if ~isempty(fmu.descriptionFMU.DefaultExperiment.startTime) then
                Tstart = fmu.descriptionFMU.DefaultExperiment.startTime;
            else
                Tstart = 0;
            end
            // retrieve the stop time from XML description
            if ~isempty(fmu.descriptionFMU.DefaultExperiment.stopTime) then
                Tend = fmu.descriptionFMU.DefaultExperiment.stopTime;
            else
                Tend = 10;
            end
            // default values for logging and dTime
            logging = %f;
            dt = 0.01;
    else
       if typeof(fmu) <> "FMU" then
          error(msprintf(_("%s: Wrong type for input argument #%d: %s expected.\n"), "fmiSimulate", 1, "FMU"));
       elseif typeof(simTime) <> 'constant' | length(simTime) <> 3 then
          error(msprintf(_('%s: Wrong type for input argument #%d: Real matrix expected.\n'), 'fmiSimulate', 2));
       elseif typeof(logger) <> 'boolean' then
          error(msprintf(_('%s: Wrong type for input argument #%d: boolean expected.\n'), 'fmiSimulate', 3));
       end
       // values for the experiment
       Tstart = simTime(1);
       Tend = simTime(3);
       dt = simTime(2);
       logging = logger;
    end
// link a library
   fmu.simulationLibrary = fmu_link(fmu.libraryFile,fmu.descriptionFMU.modelIdentifier, fmu.fmiType + " " + fmu.descriptionFMU.fmiVersion);

// arrays of final results
realOutputs = [];
integerOutputs = [];
booleanOutputs = [];
stringOutputs = []
timeOutput = [];

// check version of FMI
fmiGetVersion(fmu);
fmiGetModelTypesPlatform(fmu);

// get all value references for fmiGetXXX functions
realRefs = fmu.modelVariables.Real.valueReference(find(fmu.modelVariables.Real.alias == "noAlias"));
integerRefs = fmu.modelVariables.Integer.valueReference(find(fmu.modelVariables.Integer.alias == "noAlias"));
booleanRefs = fmu.modelVariables.Boolean.valueReference(find(fmu.modelVariables.Boolean.alias == "noAlias"));
stringRefs = fmu.modelVariables.String.valueReference(find(fmu.modelVariables.String.alias == "noAlias"));
enumRefs = fmu.modelVariables.Enumeration.valueReference(find(fmu.modelVariables.Enumeration.alias == "noAlias"));
// get value references with start values
realRefsStartPos = find(~isnan(fmu.modelVariables.Real.start) & fmu.modelVariables.Real.causality <> "input" & fmu.modelVariables.Real.causality <> "output");
realRefsStart = fmu.modelVariables.Real.valueReference(realRefsStartPos);
integerRefsStartPos = find(~isnan(fmu.modelVariables.Integer.start) & fmu.modelVariables.Integer.causality <> "input" & fmu.modelVariables.Integer.causality <> "output");
integerRefsStart = fmu.modelVariables.Integer.valueReference(integerRefsStartPos);
booleanRefsStartPos = find(fmu.modelVariables.Boolean.variability == "discrete" & fmu.modelVariables.Boolean.causality <> "input" &...
fmu.modelVariables.Boolean.alias == "noAlias" & fmu.modelVariables.Boolean.causality <> "output");
booleanRefsStart = fmu.modelVariables.Boolean.valueReference(booleanRefsStartPos);
stringRefsStartPos = find(~isempty(fmu.modelVariables.String.start) & fmu.modelVariables.String.causality <> "input");
stringRefsStart = fmu.modelVariables.String.valueReference(stringRefsStartPos);
enumRefsStartPos = find(~isnan(fmu.modelVariables.Enumeration.start) & fmu.modelVariables.Enumeration.causality <> "input");
enumRefsStart = fmu.modelVariables.Enumeration.valueReference(enumRefsStartPos);

//get start values
realStartValues = fmu.modelVariables.Real.start(realRefsStartPos);
integerStartValues = fmu.modelVariables.Integer.start(integerRefsStartPos);
booleanStartValues = fmu.modelVariables.Boolean.start(booleanRefsStartPos);
stringStartValues = fmu.modelVariables.String.start(stringRefsStartPos);
enumStartValues = fmu.modelVariables.Enumeration.start(enumRefsStartPos);
// get value references with causality == 'input'
realRefsInputPos = find(fmu.modelVariables.Real.causality == "input");
realRefsInput = fmu.modelVariables.Real.valueReference(realRefsInputPos);
integerRefsInputPos = find(fmu.modelVariables.Integer.causality == "input");
integerRefsInput = fmu.modelVariables.Integer.valueReference(integerRefsInputPos);
booleanRefsInputPos = find(fmu.modelVariables.Boolean.causality == "input" & fmu.modelVariables.Boolean.alias == "noAlias");
booleanRefsInput = fmu.modelVariables.Boolean.valueReference(booleanRefsInputPos);
stringRefsInputPos = find(fmu.modelVariables.String.causality == "input");
stringRefsInput = fmu.modelVariables.String.valueReference(stringRefsInputPos);
enumRefsInputPos = find(fmu.modelVariables.Enumeration.causality == "input");
enumRefsInput = fmu.modelVariables.Enumeration.valueReference(enumRefsInputPos);
//get start values
realInputValues = fmu.modelVariables.Real.start(realRefsInputPos);
integerInputValues = fmu.modelVariables.Integer.start(integerRefsInputPos);
booleanInputValues = fmu.modelVariables.Boolean.start(booleanRefsInputPos);
stringInputValues = fmu.modelVariables.String.start(stringRefsInputPos);
enumInputValues = fmu.modelVariables.Enumeration.start(enumRefsInputPos);
// get outputs
realRefsOutputPos = find(fmu.modelVariables.Real.causality == "output");
realRefsOutput = fmu.modelVariables.Real.valueReference(realRefsOutputPos);
integerRefsOutputPos = find(fmu.modelVariables.Integer.causality == "output");
integerRefsOutput = fmu.modelVariables.Integer.valueReference(integerRefsOutputPos);
booleanRefsOutputPos = find(fmu.modelVariables.Boolean.causality == "output");
booleanRefsOutput = fmu.modelVariables.Boolean.valueReference(booleanRefsOutputPos);
stringRefsOutputPos = find(fmu.modelVariables.String.causality == "output");
stringRefsOutput = fmu.modelVariables.String.valueReference(stringRefsOutputPos);

// instantiate model
instance = fmiInstantiateModel(fmu);

// set the start time
Tnext = Tend;
time = Tstart;

// set debug logging
fmiSetDebugLogging(instance, logging);

// set the start time
fmiSetTime(instance, time);

// Set user values for simulation at time = Tstart
if ~isempty(fmu.setValues.realNames) then
// find the value references for names of real variables
    realUserRefsPos = zeros(1:size(fmu.setValues.realNames,"*"));
    for i=1:size(fmu.setValues.realNames,"*")
        f = find(fmu.modelVariables.Real.name == fmu.setValues.realNames(i));
        if f <> [] then
           realUserRefsPos(i) = f;
        end
    end
    if or(realUserRefsPos == 0) == %t then
       error(msprintf(_('%s: Wrong name(s) %s for setting real values.\n'), 'fmiSimulate',sci2exp(fmu.setValues.realNames(find(realUserRefsPos == 0)))));
    end
    realUserRefs = fmu.modelVariables.Real.valueReference(realUserRefsPos);
    // set user values
    fmiSetReal(instance,[realUserRefs],[fmu.setValues.realValues]);
else
    // set default values
    // set all variables with start values (of "ScalarVariable / <type> / start") and "inputs"
    if ~isempty([realRefsStart,realRefsInput]) then
        fmiSetReal(instance,[realRefsStart,realRefsInput],[realStartValues,realInputValues]);
    end
end
if ~isempty(fmu.setValues.integerNames) then
// find the value references for names of integer variables
    integerUserRefsPos = zeros(1:size(fmu.setValues.integerNames,"*"));
    for i=1:size(fmu.setValues.integerNames,"*")
        f = find(fmu.modelVariables.Integer.name == fmu.setValues.integerNames(i));
        if f <> [] then
           integerUserRefsPos(i) = f;
        end
    end
    if or(integerUserRefsPos == 0) == %t then
       error(msprintf(_('%s: Wrong name(s) %s for setting string values.\n'), 'fmiSimulate',sci2exp(fmu.setValues.integerNames(find(integerUserRefsPos == 0)))));
    end
    integerUserRefs = fmu.modelVariables.Integer.valueReference(integerUserRefsPos);
    // set user values
    fmiSetInteger(instance,[integerUserRefs],[fmu.setValues.integerValues]);
else
    // set default values
    // set all variables with start values (of "ScalarVariable / <type> / start") and "inputs"
    if ~isempty([integerRefsStart,enumRefsStart,integerRefsInput,enumRefsInput]) then
        fmiSetInteger(instance,[integerRefsStart,enumRefsStart,integerRefsInput,enumRefsInput],[integerStartValues,enumStartValues,integerInputValues,enumInputValues]);
    end
end

if ~isempty(fmu.setValues.booleanNames) then
// find the value references for names of boolean variables
    booleanUserRefsPos = zeros(1:size(fmu.setValues.booleanNames,"*"));
    for i=1:size(fmu.setValues.booleanNames,"*")
        f = find(fmu.modelVariables.Boolean.name == fmu.setValues.booleanNames(i));
        if f <> [] then
           booleanUserRefsPos(i) = f;
        end
    end
    if or(booleanUserRefsPos == 0) == %t then
       error(msprintf(_('%s: Wrong name(s) %s for setting boolean values.\n'), 'fmiSimulate',sci2exp(fmu.setValues.booleanNames(find(booleanUserRefsPos == 0)))));
    end
    booleanUserRefs = fmu.modelVariables.Boolean.valueReference(booleanUserRefsPos);
    // set user values
    fmiSetBoolean(instance,[booleanUserRefs],[fmu.setValues.booleanValues]);
else
    // set default values
    // set all variables with start values (of "ScalarVariable / <type> / start") and "inputs"
    if ~isempty([booleanRefsStart,booleanRefsInput]) then
        fmiSetBoolean(instance,[booleanRefsStart,booleanRefsInput],[booleanStartValues,booleanInputValues]);
    end
end

if ~isempty(fmu.setValues.stringNames) then
// find the value references for names of string variables
    stringUserRefsPos = zeros(1:size(fmu.setValues.stringNames,"*"));
    for i=1:size(fmu.setValues.stringNames,"*")
        f = find(fmu.modelVariables.String.name == fmu.setValues.stringNames(i));
        if f <> [] then
           stringUserRefsPos(i) = f;
        end
    end
    if or(stringUserRefsPos == 0) == %t then
       error(msprintf(_('%s: Wrong name(s) %s for setting string values.\n'), 'fmiSimulate',sci2exp(fmu.setValues.stringNames(find(stringUserRefsPos == 0)))));
    end
    stringUserRefs = fmu.modelVariables.Boolean.valueReference(stringUserRefsPos);
    // set user values
    fmiSetString(instance,[stringUserRefs],[fmu.setValues.stringValues]);
else
    // set default values
    // set all variables with start values (of "ScalarVariable / <type> / start") and "inputs"
    if ~isempty([stringRefsInput,stringRefsStart]) then
        fmiSetString(instance,[stringRefsInput,stringRefsStart],[stringInputValues,stringStartValues]);
    end
end

// initialization
// there is no reason to call fmiSetXXX after this function was called for the variables which are not the 'inputs'
[eventInfo, instance] = fmiInitialize(instance, %f,0.0);

// retrieve initial state x and
// nominal values of x (if absolute tolerance is needed)
states = fmiGetContinuousStates(instance);
nom_states = fmiGetNominalContStates(instance);
events = fmiGetEventIndicators(instance);
refsState = fmiGetStateValueRefs(instance);

// retrieve solution at t=Tstart
timeOutput = Tstart;
// get VALUES
// get real
if ~isempty(fmu.getValues.realNames) then
// find the value references for names of real variables
    realUserRefsPos = zeros(1:size(fmu.getValues.realNames,"*"));
    for i=1:size(fmu.getValues.realNames,"*")
        f = find(fmu.modelVariables.Real.name == fmu.getValues.realNames(i));
        if f <> [] then
           realUserRefsPos(i) = f;
        end
    end
    if or(realUserRefsPos == 0) == %t then
       error(msprintf(_('%s: Wrong name(s) %s for getting real values.\n'), 'fmiSimulate',sci2exp(fmu.getValues.realNames(find(realUserRefsPos == 0)))));
    end
    realUserRefsGet = fmu.modelVariables.Real.valueReference(realUserRefsPos);
    // get values
    values = fmiGetReal(instance, [realUserRefsGet]);
    realOutputs(1, :) = values;
else
    values = fmiGetReal(instance, [realRefsInput,realRefsOutput,realRefsStart]);
    realOutputs(1, :) = values;
end
// get integer
if ~isempty(fmu.getValues.integerNames) then
// find the value references for names of integer variables
    integerUserRefsPos = zeros(1:size(fmu.getValues.integerNames,"*"));

    for i=1:size(fmu.getValues.integerNames,"*")
        f = find(fmu.modelVariables.Integer.name == fmu.getValues.integerNames(i));
        if f <> [] then
           integerUserRefsPos(i) = f;
        end
    end
    if or(integerUserRefsPos == 0) == %t then
       error(msprintf(_('%s: Wrong name(s) %s for getting integer values.\n'), 'fmiSimulate',sci2exp(fmu.getValues.integerNames(find(integerUserRefsPos == 0)))));
    end
    integerUserRefsGet = fmu.modelVariables.Integer.valueReference(integerUserRefsPos);
    // get values
    values = fmiGetInteger(instance, [integerUserRefsGet]);
    integerOutputs(1, :) = values;
else
    values = fmiGetInteger(instance, [integerRefsInput,integerRefsOutput,enumRefsInput,integerRefsStart,enumRefsStart]);
    integerOutputs($+1, :) = values;
end
// get boolean
if ~isempty(fmu.getValues.booleanNames) then
// find the value references for names of boolean variables
    booleanUserRefsPos = zeros(1:size(fmu.getValues.booleanNames,"*"));
    for i=1:size(fmu.getValues.booleanNames,"*")
        f = find(fmu.modelVariables.Boolean.name == fmu.getValues.booleanNames(i));
        if f <> [] then
           booleanUserRefsPos(i) = f;
        end
    end
    if or(booleanUserRefsPos == 0) == %t then
       error(msprintf(_('%s: Wrong name(s) %s for getting boolean values.\n'), 'fmiSimulate',sci2exp(fmu.getValues.booleanNames(find(booleanUserRefsPos == 0)))));
    end
    booleanUserRefsGet = fmu.modelVariables.Boolean.valueReference(booleanUserRefsPos);
    // get values
    values = fmiGetBoolean(instance, [booleanUserRefsGet]);
    booleanOutputs(1, :) = values;
else
    values = fmiGetBoolean(instance, [booleanRefsInput,booleanRefsOutput,booleanRefsStart]);
    booleanOutputs($+1, :) = values;
end

//fmiGetString is not yet supported.
//values = fmiGetString(instance, stringRefs);
//out.stringOutputs($+1, :) = values;

// get user inputs
inputTypes = ones(1:size(fmu.setValues.inputs,"*"));
if ~isempty(fmu.setValues.inputs) then
    refsInputs = inputTypes;
    for i=1:size(fmu.setValues.inputs,"*")
        if find(fmu.modelVariables.Real.name == fmu.setValues.inputs(i)) then
            refsInputs(i) = fmu.modelVariables.Real.valueReference(find(fmu.modelVariables.Real.name == fmu.setValues.inputs(i)));
            inputTypes(i) = 1; //'real'
        elseif find(fmu.modelVariables.Integer.name == fmu.setValues.inputs(i)) then
            refsInputs(i) = fmu.modelVariables.Integer.valueReference(find(fmu.modelVariables.Integer.name == fmu.setValues.inputs(i)));
            inputTypes(i) = 2; //'integer'
        elseif find(fmu.modelVariables.Boolean.name == fmu.setValues.inputs(i)) then
            refsInputs(i) = fmu.modelVariables.Boolean.valueReference(find(fmu.modelVariables.Boolean.name == fmu.setValues.inputs(i)));
            inputTypes(i) = 3; //'boolean'
        elseif find(fmu.modelVariables.String.name == fmu.setValues.inputs(i)) then
            refsInputs(i) = fmu.modelVariables.String.valueReference(find(fmu.modelVariables.String.name == fmu.setValues.inputs(i)));
            inputTypes(i) = 4; //'string'
        end
    end
end

// precompute the input time series once (avoid evstr in the time loop)
inputSeries = list();
if ~isempty(inputTypes) then
    for i=1:length(inputTypes)
        inputSeries(i) = evstr('fmu.setValues.input_'+string(i)+'_values');
    end
end

//fmu.setValues.realValues
while (time < Tend & eventInfo.terminateSimulation == %f)

    // advance time
    h = min(dt, Tnext - time);
    time = time + h;
    fmiSetTime(instance, time);
    //fmiSetReal(instance,[refsInputs(i)],[evstr('fmu.setValues.input_'+string(i)+'_values(1,t_index)')]);
    // set user inputs at t = time
    if ~isempty(inputTypes) then
        for i=1:length(inputTypes)
            series = inputSeries(i);
            if ~isempty(series) & ~isempty(series(1,:)) & ~isempty(series(2,:)) then
                t_index = find(abs(time-series(2,:))<1e-12 == %t);
                if ~isempty(t_index) then
                    select inputTypes(i)
                    case 1 then fmiSetReal(instance,[refsInputs(i)],[series(1,t_index)]);
                    case 2 then fmiSetInteger(instance,[refsInputs(i)],[series(1,t_index)]);
                    case 3 then fmiSetBoolean(instance,[refsInputs(i)],[series(1,t_index)==1]);
                    case 4 then fmiSetString(instance,[refsInputs(i)],[series(1,t_index)]);
                    end
                end
            end
        end
    end
    // compute derivatives
    derivatives = fmiGetDerivatives(instance);
    // set states at t = time (perform one step)
    states = states + h*derivatives // forward Euler method
    fmiSetContinuousStates(instance, states);
    // get event indicators at t = time
    events_next = fmiGetEventIndicators(instance);
    // inform the model about an accepted step
    callEventUpdate = fmiCompletedIntegrStep(instance);

    // handle events, if any
    time_event = abs(time - Tnext) <= %eps;

    if fmu.descriptionFMU.numberOfEventIndicators > 0 then
        state_event = or(events & (events .* events_next < 0));// compare sign of z with previous z, simple hysteresis implementation
        events = events_next;
    else
        state_event = %f;
    end

    if or([callEventUpdate time_event state_event]) then
        eventInfo.iterationConverged = %f;

        while eventInfo.iterationConverged == %f  //event iteration
            eventInfo = fmiEventUpdate(instance, %t);

            // retrieve solution at every event iteration
            if eventInfo.iterationConverged == %t then
                timeOutput($+1) = time;
                // get Real
                if ~isempty(fmu.getValues.realNames) then
                    values = fmiGetReal(instance, [realUserRefsGet]);
                    realOutputs($+1, :) = values;
                else
                    values = fmiGetReal(instance, [realRefsInput,realRefsOutput,realRefsStart]);
                    realOutputs($+1, :) = values;
                end
                // get Integer
                if ~isempty(fmu.getValues.integerNames) then
                    values = fmiGetInteger(instance, [integerUserRefsGet]);
                    integerOutputs($+1, :) = values;
                else
                    values = fmiGetInteger(instance, [integerRefsInput,integerRefsOutput,enumRefsInput,integerRefsStart,enumRefsStart]);
                    integerOutputs($+1, :) = values;
                end
                // get boolean
                if ~isempty(fmu.getValues.booleanNames) then
                    values = fmiGetBoolean(instance, [booleanUserRefsGet]);
                    booleanOutputs($+1, :) = values;
                else
                    values = fmiGetBoolean(instance, [booleanRefsInput,booleanRefsOutput,booleanRefsStart]);
                    booleanOutputs($+1, :) = values;
                end
                //values = fmiGetString(instance, stringRefs);
                //out.stringOutputs($+1, :) = values;
            end
        end

        if eventInfo.stateValuesChanged then
            //the model signals a value change of states, retrieve them
            states = fmiGetContinuousStates(instance);
        end

        if eventInfo.stateValueReferencesChanged then
            //the meaning of states has changed; retrieve new nominal values
            nominal = fmiGetNominalContStates(instance);
        end

        if eventInfo.upcomingTimeEvent then
            Tnext = min(eventInfo.nextEventTime, Tend);
        else
            Tnext = Tend;
        end
    end
    // Retrieve solution at t=time
    timeOutput($+1) = time;
    // get Real
    if ~isempty(fmu.getValues.realNames) then
        values = fmiGetReal(instance, [realUserRefsGet]);
        realOutputs($+1, :) = values;
    else
        values = fmiGetReal(instance, [realRefsInput,realRefsOutput,realRefsStart]);
        realOutputs($+1, :) = values;
    end
    // get Integer
    if ~isempty(fmu.getValues.integerNames) then
        values = fmiGetInteger(instance, [integerUserRefsGet]);
        integerOutputs($+1, :) = values;
    else
        values = fmiGetInteger(instance, [integerRefsInput,integerRefsOutput,enumRefsInput,integerRefsStart,enumRefsStart]);
        integerOutputs($+1, :) = values;
    end
    // get boolean
    if ~isempty(fmu.getValues.booleanNames) then
        values = fmiGetBoolean(instance, [booleanUserRefsGet]);
        booleanOutputs($+1, :) = values;
    else
        values = fmiGetBoolean(instance, [booleanRefsInput,booleanRefsOutput,booleanRefsStart]);
        booleanOutputs($+1, :) = values;
    end
    //values = fmiGetString(instance, stringRefs);
    //out.stringOutputs($+1, :) = values;
end
// retrieve final values
// get Real
if ~isempty(fmu.getValues.realNames) then
    values = fmiGetReal(instance, [realUserRefsGet]);
    realOutputs($+1, :) = values;
    // get the names of variables(start and inputs)
    namesReal = fmu.getValues.realNames;
else
    values = fmiGetReal(instance, [realRefsInput,realRefsOutput,realRefsStart]);
    realOutputs($+1, :) = values;
    // get the names of variables(start and inputs)
    namesReal = fmu.modelVariables.Real.name([realRefsInputPos,realRefsOutputPos,realRefsStartPos]);
end
// get Integer
if ~isempty(fmu.getValues.integerNames) then
    values = fmiGetInteger(instance, [integerUserRefsGet]);
    integerOutputs($+1, :) = values;
    namesInteger = fmu.getValues.integerNames;
else
    values = fmiGetInteger(instance, [integerRefsInput,integerRefsOutput,enumRefsInput,integerRefsStart,enumRefsStart]);
    integerOutputs($+1, :) = values;
    namesInteger = fmu.modelVariables.Integer.name([integerRefsInputPos,integerRefsOutputPos,enumRefsInputPos,integerRefsStartPos,enumRefsStartPos]);
end
// get boolean
if ~isempty(fmu.getValues.booleanNames) then
    values = fmiGetBoolean(instance, [booleanUserRefsGet]);
    booleanOutputs($+1, :) = values;
    namesBoolean = fmu.getValues.booleanNames;
else
    values = fmiGetBoolean(instance, [booleanRefsInput,booleanRefsOutput,booleanRefsStart]);
    booleanOutputs($+1, :) = values;
    namesBoolean = fmu.modelVariables.Boolean.name([booleanRefsInputPos,booleanRefsOutputPos,booleanRefsStartPos]);
end

// terminate the simulation
fmiTerminate(instance);

//values = fmiGetString(instance, stringRefs);
//out.stringOutputs($+1, :) = values;
// string
namesString = fmu.modelVariables.String.name([stringRefsInputPos,stringRefsOutputPos,stringRefsStartPos]);

// creation of structures
Real = [];
for i=1:size(namesReal,"*")
    execstr('Real'+'(i)'+' = struct('"result'",struct('"name'",namesReal(i),'"values'", realOutputs(:,i), '"time'", timeOutput))');
end
Integer = [];
for i=1:size(namesInteger,"*")
    execstr('Integer'+'(i)'+' = struct('"result'",struct('"name'",namesInteger(i),'"values'", integerOutputs(:,i), '"time'", timeOutput))');
end
Boolean = [];
for i=1:size(namesBoolean,"*")
    execstr('Boolean'+'(i)'+' = struct('"result'",struct('"name'",namesBoolean(i),'"values'", booleanOutputs(:,i), '"time'", timeOutput))');
end
String = [];
for i=1:size(namesString,"*")
    execstr('String'+'(i)'+' = struct('"result'",struct('"name'",namesString(i),'"values'", stringOutputs(:,i), '"time'", timeOutput))');
end

// final structure of results
out = struct("Real",Real,"Integer",Integer,"Boolean",Boolean,"String",String);

// cleanup
fmiFreeModelInstance(instance);
endfunction
