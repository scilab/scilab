// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- XCOS TEST -->
// <-- NO CHECK REF -->
//
// <-- Non-regression test for bug 7202 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/7202
//
// <-- Short Description -->
// It's not possible to send string parameters via the opar structure of an xcos
// block.

function [x,y,typ] = CUSTOM_7202(job,arg1,arg2)
    x=[];
    y=[];
    typ=[];
    select job
    case "set" then
        x=arg1;
        x.model.opar(1) = "this is a string parameter"
    case "define" then
        model=scicos_model();
        model.sim="trash";
        model.dep_ut = [%t %f];
        model.opar(1) = "this is a string parameter"
        x=standard_define([2 3], model, "", []);
    end
endfunction

scs_m = scicos_diagram();
scs_m.props.tf = 1;
scs_m.objs(1) = CUSTOM_7202("define");

// check the simulation
scicos_simulate(scs_m);

// save, load and check
fname = fullfile(TMPDIR, "sample.zcos");
xcosDiagramToScilab(fname, scs_m);
prev_scs_m = xcosDiagramToScilab(fname);

assert_checkequal(scs_m.objs(1).model.opar(1), prev_scs_m.objs(1).model.opar(1));
assert_checkequal(scs_m.objs(1).model.opar(1), "this is a string parameter");

