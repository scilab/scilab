// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 201é - Clément DAVID
//
// This file is distributed under the same license as the Scilab package.

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->
//
// <-- Short Description -->
// Check the API of multiple Xcos hooks, see SEP_066_Xcos_hooks.odt

global status;
status = [];

function continueSimulation = pre1(scs_m, needcompile)
    global status;

    [linenum,mac]=where();
    status = [status mac(1)];
    continueSimulation=%t;
endfunction

function continueSimulation = pre2(scs_m, needcompile)
    global status;

    [linenum,mac]=where();
    status = [status mac(1)];
    continueSimulation=%t;
endfunction

function continueSimulation = pre3(scs_m, needcompile)
    global status;

    [linenum,mac]=where();
    status = [status mac(1)];
    continueSimulation=%t;
endfunction

function post1(%cpr, scs_m, needcompile)
    global status;

    [linenum,mac]=where();
    status = [status mac(1)];
endfunction

function post2(%cpr, scs_m, needcompile)
    global status;

    [linenum,mac]=where();
    status = [status mac(1)];
endfunction

// register the hooks
pre_xcos_simulate=list("pre1", "pre2", "pre3");
post_xcos_simulate=list("post1", "post2");

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/demos/Simple_Demo.zcos"));
xcos_simulate(scs_m, 4);

assert_checkequal(status, ["pre1", "pre2", "pre3", "post1", "post2"]);


