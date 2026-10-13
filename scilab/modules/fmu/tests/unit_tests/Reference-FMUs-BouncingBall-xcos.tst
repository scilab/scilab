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
// Unit test for Reference-FMUs BouncingBall
//

if ~isdef("Branchinglib") then
    loadXcosLibs
end

path = fmigetPath();

function assert_checkresults(results, fmuImpl)
    // helper function to assert the simulation results ; this should be fast and numerically stable
    if getscilabmode() == "STD" then
        // this is a user-launched test, display the results
        scf();
        plot(results.time, results.values);
        title(basename(schemapath) + " " + fmuImpl);
    end

    h = results.values(:, 1);
    v = results.values(:, 2);
    
    // initial value from modelDescription.xml are:
    assert_checkequal(h(1), 1);
    assert_checkequal(v(1), 0);

    // this is bouncing down
    [maxi, maxi_index] = max(h);
    assert_checkequal(maxi_index, 1);
    assert_checkalmostequal(maxi, 1.0);
    assert_checktrue(v(1:10) <= 0);
    
    // this is never down zero
    [mini, mini_index] = min(h);
    
    // down to 0 after a few iteration and have a no speed
    touchdown_index = find(abs(h) < %eps);
    final_touchdown_index = touchdown_index(diff(touchdown_index)==1);

    assert_checktrue(final_touchdown_index > 50);
    assert_checkalmostequal(v(final_touchdown_index), zeros(final_touchdown_index)', scs_m.props.tol(2), scs_m.props.tol(1));
endfunction


//
// FMU v2 ModelExchange in nwni mode, enforce a non graphic setup for potential valgrind / ASAN checking
//
schemapath = fullfile(path, "tests", "unit_tests", "Reference-FMUs", "BouncingBall.xcos");
fmupath = fullfile(path, "tests", "unit_tests", "Reference-FMUs","2.0","BouncingBall.fmu");

function scs_m = create_schema()
    %scicos_context=struct();
    scicos_getvalue=setvalue;
    getvalue=setvalue;
    global %scicos_prob;
    %scicos_prob=%f;

    scs_m = scicos_diagram(props=scicos_params(tf=3));
    scs_m.props.tol(4) = 1e-2; // enforce a specific deltat to trigger zero-crossing

    b1 = FMU_WRAPPER("define");
    b1.graphics.exprs = [fmupath; "" ; "me 2.0"];
    b1 = FMU_WRAPPER("set", b1);

    b2 = MUX("define");

    b3 = TOWS_c("define");
    b3.graphics.exprs = ["1024"; "results" ; "1"];
    b3 = TOWS_c("set", b3);

    b4 = SPLIT_f("define");

    b5 = TOWS_c("define");
    b5.graphics.exprs = ["1024"; "event_results" ; "0"];
    b5 = TOWS_c("set", b5);

    b6 = CLOCK_c("define");

    l1 = scicos_link(from=[1 1 0], to=[2 1 1]);
    l2 = scicos_link(from=[1 2 0], to=[2 2 1]);
    l3 = scicos_link(from=[2 1 0], to=[4 1 1]);
    l4 = scicos_link(from=[4 1 0], to=[3 1 1]);
    l5 = scicos_link(from=[4 2 0], to=[5 1 1]);
    l6 = scicos_link(from=[6 1 0], to=[5 1 1], ct=[1 -1]);

    scs_m.objs = list(b1, b2, b3, b4, b5, b6, l1, l2, l3, l4, l5, l6);
endfunction

scs_m = create_schema();
Info = scicos_simulate(scs_m, "nw");
assert_checkresults(results, "me 2.0");

//
// FMU v2
// 
schemapath = fullfile(path, "tests", "unit_tests", "Reference-FMUs", "BouncingBall.xcos");
fmupath = fullfile(path, "tests", "unit_tests", "Reference-FMUs","2.0","BouncingBall.fmu");

// check CoSimulation v2
[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(fmupath, [], "cs 2.0");
importXcosDiagram(schemapath);

assert_checkequal(scs_m.objs(1).gui, "FMU_WRAPPER");
scs_m.objs(1).graphics.exprs = [fmupath; "" ; "cs 2.0"];

Info = scicos_simulate(scs_m, "nw");
assert_checkresults(results, "cs 2.0");

// check ModelExchange v2
[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(fmupath, [], "me 2.0");
importXcosDiagram(schemapath);

assert_checkequal(scs_m.objs(1).gui, "FMU_WRAPPER");
scs_m.objs(1).graphics.exprs = [fmupath; "" ; "me 2.0"];

Info = scicos_simulate(scs_m, "nw");
assert_checkresults(results, "me 2.0");

//
// FMU v3
//
schemapath = fullfile(path, "tests", "unit_tests", "Reference-FMUs", "BouncingBall.xcos");
fmupath = fullfile(path, "tests", "unit_tests", "Reference-FMUs","3.0","BouncingBall.fmu");

// check CoSimulation v3
[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(fmupath, [], "cs 3.0");
importXcosDiagram(schemapath);

assert_checkequal(scs_m.objs(1).gui, "FMU_WRAPPER");
scs_m.objs(1).graphics.exprs = [fmupath; "" ; "cs 3.0"];

Info = scicos_simulate(scs_m, "nw");
assert_checkresults(results, "cs 3.0");

// check ModelExchange v3
[workdir, xml, libname, modelIdentifier, fmuImpl] = loadFMU(fmupath, [], "me 3.0");
importXcosDiagram(schemapath);

assert_checkequal(scs_m.objs(1).gui, "FMU_WRAPPER");
scs_m.objs(1).graphics.exprs = [fmupath; "" ; "me 3.0"];

Info = scicos_simulate(scs_m, "nw");
assert_checkresults(results, "me 3.0");
