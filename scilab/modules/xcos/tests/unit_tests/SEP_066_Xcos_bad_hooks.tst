// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
//
// This file is distributed under the same license as the Scilab package.

// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->
//
// <-- Short Description -->
// Check the API of the Xcos hooks, see SEP_066_Xcos_hooks.odt

global status;
status = [];

function continueSimulation = pre_xcos_simulate(scs_m, needcompile)
    global status;

    1 + "string"
    status = [status "pre_called"];
endfunction

function post_xcos_simulate(%cpr, scs_m, needcompile)
    global status;

    1 + "string"
    status = [status "post_called"];
endfunction

assert_checktrue(importXcosDiagram(SCI + "/modules/xcos/demos/Simple_Demo.zcos"));
xcos_simulate(scs_m, 4);

assert_checkequal(status, []);
