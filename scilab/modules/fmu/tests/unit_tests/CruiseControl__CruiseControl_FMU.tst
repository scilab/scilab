//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2013 - Scilab Enterprises - Clement David
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//

// <-- NO CHECK REF -->

// ****** Model setup ****** //

path = fmigetPath();
fmu = importFMU(fullfile(path, "tests", "unit_tests", "SCADE_CruiseControl__CruiseControl_FMU.fmu"));

// get the model description
desc = fmu.descriptionFMU;
// get the variables
vars = fmu.modelVariables;
// link the library
library = fmu_link(fmu.libraryFile, desc.modelIdentifier);
// instantiate the model
m = fmu_call(library, "fmiInstantiateModel", desc.modelIdentifier, desc.guid, %f);

nx = desc.numberOfContinuousStates;
nz = desc.numberOfEventIndicators;
Tstart = 0;
Tend = 10;
dt = 0.01;

// ****** Scenario setup ****** //

// define the test scenario for each type
realInput = find(vars.Real.causality == "input");
realInName = vars.Real.name(realInput);
realInRef = vars.Real.valueReference(realInput);
boolInput = find(vars.Boolean.causality == "input");
boolInName = vars.Boolean.name(boolInput);
boolInRef = vars.Boolean.valueReference(boolInput);
realOutput = find(vars.Real.causality == "output");
realOutName = vars.Real.name(realOutput);
realOutRef = vars.Real.valueReference(realOutput);
integerOutput = find(vars.Integer.causality == "output");
integerOutName = vars.Integer.name(integerOutput);
integerOutRef = vars.Real.valueReference(integerOutput);

// define variables with references value to ease scenario definition
execstr(realInName + " = " + string(1:size(realInName, '*')) + ";");
execstr(boolInName + " = " + string(1:size(boolInName, '*')) + ";");
execstr(realOutName + " = " + string(1:size(realOutName, '*')) + ";");
execstr(integerOutName + " = " + string(1:size(integerOutName, '*')) + ";");

// scenario initial values
timeInput = [];
realInputs = zeros(realInName);
boolInputs = 1 == zeros(boolInName);
timeOutput = [];
realOutputs = [];
IntegerOutputs = [];

// scenario
timeInput(1) = 0;
realInputs(1, Speed) = 0;
boolInputs(1, On) = %t;
timeInput($+1) = 3;
realInputs($+1, Speed) = 0;
boolInputs($+1, On) = %t;
timeInput($+1) = 3+dt;
realInputs($+1, Speed) = 20;
boolInputs($+1, On) = %t;
timeInput($+1) = 6;
realInputs($+1, Speed) = 20;
boolInputs($+1, On) = %t;
timeInput($+1) = 6+dt;
realInputs($+1, Speed) = 10;
boolInputs($+1, On) = %t;
timeInput($+1) = Tend;
realInputs($+1, Speed) = 10;
boolInputs($+1, On) = %t;

// used to check the scenario
// plot(wholeTime,realInputs);

// ****** FMU algorithm ****** //

// set the start time
Tnext = Tend;
time = Tstart;
status = fmu_call(library, 'fmiSetTime', m, time);
assert_checkequal(status, 0);

// set all variable start values (of "ScalarVariable / <type> / start")
status = fmu_call(library, 'fmiSetReal', m, vars.Real.valueReference, vars.Real.start);
assert_checkequal(status, 0);
status = fmu_call(library, 'fmiSetInteger', m, vars.Integer.valueReference, vars.Integer.start);
assert_checkequal(status, 0);
status = fmu_call(library, 'fmiSetBoolean', m, vars.Boolean.valueReference, vars.Boolean.start);
assert_checkequal(status, 0);
// set the input values at time = Tstart
status = fmu_call(library, 'fmiSetReal', m, realInRef, realInputs(1, :));
assert_checkequal(status, 0);
status = fmu_call(library, 'fmiSetBoolean', m, boolInRef, boolInputs(1, :));
assert_checkequal(status, 0);

// initialize
[status, eventInfo] = fmu_call(library, 'fmiInitialize', m, %f, 0.0);
assert_checkequal(status, 0);

// retrieve initial state x and
// nominal values of x (if absolute tolerance is needed)
[status, x] = fmu_call(library, 'fmiGetContinuousStates', m, nx);
assert_checkequal(status, 0);
[status, x_nominal] = fmu_call(library, 'fmiGetNominalContinuousStates', m, nx);
assert_checkequal(status, 0);
[status, z] = fmu_call(library, 'fmiGetEventIndicators', m, nz);
assert_checkequal(status, 0);

// retrieve solution at t=Tstart, e.g. for outputs
timeOutput = Tstart;
[status, v] = fmu_call(library, 'fmiGetReal', m, realOutRef);
assert_checkequal(status, 0);
realOutputs(1,:) = v;
[status, v] = fmu_call(library, 'fmiGetInteger', m, integerOutRef);
assert_checkequal(status, 0);
integerOutputs(1,:) = v;

// hand-crafted iteration loop from the specification, may be slower than calling dae or ode
while ( time < Tend & eventInfo.terminateSimulation == %f )

    // compute derivatives
    [status, der_x] = fmu_call(library, 'fmiGetDerivatives', m, nx);
    assert_checkequal(status, 0);

    // advance time
    h = min(dt, Tnext - time);
    time = time + h;
    status = fmu_call(library, 'fmiSetTime', m, time);
    assert_checkequal(status, 0);

    // set inputs at t = time
    t_index = find(timeInput < time) +1;
    status = fmu_call(library, 'fmiSetReal', m, realInRef, realInputs(t_index($), :));
    assert_checkequal(status, 0);
    status = fmu_call(library, 'fmiSetBoolean', m, boolInRef, boolInputs(t_index($), :));
    assert_checkequal(status, 0);

    // set states at t = time (perform one step)
    x = x + h*der_x; // forward Euler method
    status = fmu_call(library, 'fmiSetContinuousStates', m, x, nx);
    assert_checkequal(status, 0);

    // get event indicators at t = time
    [status, z_next] = fmu_call(library, 'fmiGetEventIndicators', m, nz);
    assert_checkequal(status, 0);

    // inform the model about an accepted step
    [status, callEventUpdate] = fmu_call(library, 'fmiCompletedIntegratorStep', m);
    assert_checkequal(status, 0);

    // handle events, if any
    time_event = abs(time - Tnext) <= %eps;
    if nz > 0 then
        state_event = or((z_next > 0 & z <= -%eps) & (z_next <= 0 & z > %eps)); z = z_next;  // compare sign of z with previous z, simple hysteresis implementation
    else
        state_event = %f;
    end
    if or([callEventUpdate time_event state_event]) then
        eventInfo.iterationConverged = %f;

        while eventInfo.iterationConverged == %f  //event iteration
            [status, eventInfo] = fmu_call(library, 'fmiEventUpdate', m, %t);
            assert_checkequal(status, 0);

            // retrieve solution at every event iteration
            if eventInfo.iterationConverged == %f then
                timeOutput($+1) = time;
                [status, v] = fmu_call(library, 'fmiGetReal', m, realOutRef);
                assert_checkequal(status, 0);
                realOutputs($+1, :) = v;
                [status, v] = fmu_call(library, 'fmiGetInteger', m, integerOutRef);
                assert_checkequal(status, 0);
                integerOutputs($+1, :) = v;
            end
        end

        if eventInfo.stateValuesChanged then
            //the model signals a value change of states, retrieve them
            [status, x] = fmu_call(library, 'fmiGetContinuousStates', m, nx);
            assert_checkequal(status, 0);
        end

        if eventInfo.stateValueReferencesChanged then
            //the meaning of states has changed; retrieve new nominal values
            [status, x_nominal] = fmu_call(library, 'fmiGetNominalContinuousStates', m, nx);
            assert_checkequal(status, 0);
        end

        if eventInfo.upcomingTimeEvent then
            Tnext = min(eventInfo.nextEventTime, Tend);
        else
            Tnext = Tend;
        end
    end

    timeOutput($+1) = time;
    [status, v] = fmu_call(library, 'fmiGetReal', m, realOutRef);
    assert_checkequal(status, 0);
    realOutputs($+1, :) = v;
    [status, v] = fmu_call(library, 'fmiGetInteger', m, integerOutRef);
    assert_checkequal(status, 0);
    integerOutputs($+1, :) = v;
end

// terminate simulation and retrieve final values
status = fmu_call(library, 'fmiTerminate', m);
assert_checkequal(status, 0);
[status, v] = fmu_call(library, 'fmiGetReal', m, realOutRef);
assert_checkequal(status, 0);
realOutputs($+1, :) = v;
[status, v] = fmu_call(library, 'fmiGetInteger', m, integerOutRef);
assert_checkequal(status, 0);
integerOutputs($+1, :) = v;

// cleanup
fmu_call(library, 'fmiFreeModelInstance', m);
// ulink the library
fmu_ulink(library);

// used to check the scenario
// plot(wholeTime,realOutputs);
