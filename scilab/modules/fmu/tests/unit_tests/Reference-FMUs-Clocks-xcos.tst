//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2024 - Dassault Systèmes - Clement DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
// <-- NO CHECK REF -->
// <-- NOT FIXED -->
//
// Unit test for Reference-FMUs Clocks
//
// ScheduledExecution FMUs can not be simulated by the FMU_WRAPPER block :
// the Clocks.fmu only implements ScheduledExecution and there is no
// fmu3_simulate_se simulation function.
//

if ~isdef("Branchinglib") then
    loadXcosLibs
end

path = fmigetPath();
schemapath = fullfile(path, "tests", "unit_tests", "Reference-FMUs", "Clocks.xcos");


function assert_checkresults(block_inputs, block_outputs, fmuImpl)
    // helper function to assert the simulation results ; this should be fast and numerically stable
    if getscilabmode() == "STD" then
        // this is a user-launched test, display the results
        scf();
        plot(results.time, results.values);
        title(basename(schemapath) + " " + fmuImpl);
    end
    assert_checkequal(block_inputs.time, block_outputs.time);
    assert_checkequal(block_inputs.values, block_outputs.values);
endfunction

//
// FMU v3
//
fmupath = fullfile(path, "tests", "unit_tests", "Reference-FMUs","3.0","Clocks.fmu");

// check ScheduledExecution v3
[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(fmupath, [], "se 3.0");
importXcosDiagram(schemapath);

assert_checkequal(scs_m.objs(1).gui, "FMU_WRAPPER");
scs_m.objs(1).graphics.exprs = [fmupath; "" ; "se 3.0"];

scicos_simulate(scs_m, "nw");
assert_checkresults(block_inputs, block_outputs, "se 3.0");
