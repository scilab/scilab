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
//
// Unit test for Reference-FMUs Feedthrough
//

if ~isdef("Branchinglib") then
    loadXcosLibs
end

path = fmigetPath();

function assert_checkresults(block_inputs, block_outputs, fmuImpl)
    // helper function to assert the simulation results ; this should be fast and numerically stable
    if getscilabmode() == "STD" then
        // this is a user-launched test, display the results
        scf();
        plot(block_inputs.time, block_outputs.values - block_inputs.values );
        title(basename(schemapath) + " " + fmuImpl + " diff");
    end

    assert_checkequal(block_inputs.time, block_outputs.time);
    assert_checkalmostequal(block_inputs.values, block_outputs.values, 1e-8, 1e-6);
endfunction

//
// FMU v2
//
schemapath = fullfile(path, "tests", "unit_tests", "Reference-FMUs", "Feedthrough.xcos");
fmupath = fullfile(path, "tests", "unit_tests", "Reference-FMUs","2.0","Feedthrough.fmu");

// check CoSimulation v2
[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(fmupath, [], "cs 2.0");
importXcosDiagram(schemapath);

assert_checkequal(scs_m.objs(1).gui, "FMU_WRAPPER");
scs_m.objs(1).graphics.exprs = [fmupath; "" ; "cs 2.0"];

scicos_simulate(scs_m, "nw");
assert_checkresults(block_inputs, block_outputs, "cs 2.0");

// check ModelExchange v2
[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(fmupath, [], "me 2.0");
importXcosDiagram(schemapath);

assert_checkequal(scs_m.objs(1).gui, "FMU_WRAPPER");
scs_m.objs(1).graphics.exprs = [fmupath; "" ; "me 2.0"];

scicos_simulate(scs_m, "nw");
assert_checkresults(block_inputs, block_outputs, "me 2.0");

//
// FMU v3
//
schemapath = fullfile(path, "tests", "unit_tests", "Reference-FMUs", "Feedthrough_3.0.xcos");
fmupath = fullfile(path, "tests", "unit_tests", "Reference-FMUs","3.0","Feedthrough.fmu");

// check CoSimulation v3
[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(fmupath, [], "cs 3.0");
importXcosDiagram(schemapath);

assert_checkequal(scs_m.objs(1).gui, "FMU_WRAPPER");
scs_m.objs(1).graphics.exprs = [fmupath; "" ; "cs 3.0"];

scicos_simulate(scs_m, "nw");
assert_checkresults(block_inputs, block_outputs, "cs 3.0");

// check ModelExchange v3
[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(fmupath, [], "me 3.0");
importXcosDiagram(schemapath);

assert_checkequal(scs_m.objs(1).gui, "FMU_WRAPPER");
scs_m.objs(1).graphics.exprs = [fmupath; "" ; "me 3.0"];

scicos_simulate(scs_m, "nw");
assert_checkresults(block_inputs, block_outputs, "me 3.0");
