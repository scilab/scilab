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
// Unit test for Reference-FMUs VanDerPol
//

if ~isdef("Branchinglib") then
    loadXcosLibs
end

path = fmigetPath();
schemapath = fullfile(path, "tests", "unit_tests", "Reference-FMUs", "VanDerPol.xcos");


function assert_checkresults(results, fmuImpl)
    // helper function to assert the simulation results ; this should be fast and numerically stable
    if getscilabmode() == "STD" then
        // this is a user-launched test, display the results
        scf();
        plot(results.time, results.values);
        title(basename(schemapath) + " " + fmuImpl);
    end
    
    // the same FMU should provide almost equals results to the Xcos model
    fmu_results = results.values(:, 1:2);
    xcos_results = results.values(:, 3:4);

    // check that start points are the same
    assert_checkequal(fmu_results(1,:), xcos_results(1,:));

    // check that the shape is correct at the start
    if part(fmuImpl, 1:2) == "cs" then
        // the DoStep method is using a forward Euler with step=1e-2, check the first results
        euler_step = 1e-2
        assert_checkalmostequal(fmu_results(1:50,:), xcos_results(1:50,:), sqrt(euler_step), euler_step)
    else
        assert_checkalmostequal(fmu_results, xcos_results, 1e-6, 1e-6);
    end
endfunction

//
// FMU v2
//
fmupath = fullfile(path, "tests", "unit_tests", "Reference-FMUs","2.0","VanDerPol.fmu");

// check CoSimulation v2
[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(fmupath, [], "cs 2.0");
importXcosDiagram(schemapath);

assert_checkequal(scs_m.objs(1).gui, "FMU_WRAPPER");
scs_m.objs(1).graphics.exprs = [fmupath; "" ; "cs 2.0"];

scicos_simulate(scs_m, "nw");
assert_checkresults(results, "cs 2.0");

// check ModelExchange v2
[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(fmupath, [], "me 2.0");
importXcosDiagram(schemapath);

assert_checkequal(scs_m.objs(1).gui, "FMU_WRAPPER");
scs_m.objs(1).graphics.exprs = [fmupath; "" ; "me 2.0"];

scicos_simulate(scs_m, "nw");
assert_checkresults(results, "me 2.0");

//
// FMU v3
//
fmupath = fullfile(path, "tests", "unit_tests", "Reference-FMUs","3.0","VanDerPol.fmu");

// check CoSimulation v3
[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(fmupath, [], "cs 3.0");
importXcosDiagram(schemapath);

assert_checkequal(scs_m.objs(1).gui, "FMU_WRAPPER");
scs_m.objs(1).graphics.exprs = [fmupath; "" ; "cs 3.0"];

scicos_simulate(scs_m, "nw");
assert_checkresults(results, "cs 3.0");

// check ModelExchange v3
[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(fmupath, [], "me 3.0");
importXcosDiagram(schemapath);

assert_checkequal(scs_m.objs(1).gui, "FMU_WRAPPER");
scs_m.objs(1).graphics.exprs = [fmupath; "" ; "me 3.0"];

scicos_simulate(scs_m, "nw");
assert_checkresults(results, "me 3.0");

