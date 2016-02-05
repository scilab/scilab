// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clement DAVID
// Copyright (C) 2012 - Scilab Enterprises - Clement DAVID
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- ENGLISH IMPOSED -->
// <-- XCOS TEST -->
//
// <-- Short Description -->
// Check that the simulation function should exists.


defaultlibs = ["Branching",
"Linear",
"Misc",
"Sources",
"Events",
"Integerop",
"Matrixop",
"Nonlinear",
"Sinks",
"Threshold"];

defaultlibs  = defaultlibs + "lib";
notTested = ["SUPER_f", "DSUPER", ..              // Specific blocks
"IN_f", "OUT_f", "INIMPL_f", "OUTIMPL_f",..
"CLKIN_f", "CLKINV_f", "CLKOUT_f", "CLKOUTV_f",..
"VirtualCLK0",..
"SPLIT_f", "CLKSPLIT_f",..
"TEXT_f", "PAL_f", "DEBUG", "DEBUG_SCICOS"..
"SIGNUM", "MAXMIN", "ABS_VALUE", ..    // buggy blocks
"PENDULUM_ANIM", "BPLATFORM", "MBLOCK", ..
"TKSCALE", "BARXY", ..
"SLIDER_f", "WFILE_f", "func_block" ..  // Deprecated blocks
"MPBLOCK", "fortran_block", ..       // blocks with code generation
"CBLOCK", "scifunc_block", "scifunc_block_m"];

invalidFunctions = unique(gsort(["csuper", "junk", ..
"clkfrom", "clkgoto", ..
"goto", "from", "gotomo", "frommo", ..
"limpsplit", "sampleclk", ..
"gototagvisibility", "clkgototagvisibility", "gototagvisibilitymo"]));

funcprot(0);
ilib_verbose(0);

// Stubbing the x_mdialog method
// checking it's arguments size only
function [result]=x_mdialog(title, labelsv, labelsh, default_inputs_vector)
    [lhs, rhs] = argn();
    if rhs == 3 then
        default_inputs_vector = labelsh;
        result = x_dialog(labelsv, default_inputs_vector);
    elseif rhs == 4 then
        vSize = size(labelsv, "*");
        hSize = size(labelsh, "*");
        if size(default_inputs_vector) <> [vSize, hSize] then
        mprintf("%s\nError: dialog wrong size.", cmd); pause, end;
        result = default_inputs_vector;
    else
    mprintf("%s\nError: dialog wrong size.", cmd); pause, end;
endfunction

// Stubbing the x_dialog method
// checking it's arguments size only
function [result]=x_dialog(labels, default_inputs_vector)
    if(or(size(labels) <> size(default_inputs_vector))) then
    mprintf("%s\nError: dialog wrong size.", cmd); pause, end;
    result = default_inputs_vector;
endfunction

// Stubbing the dialog method
// checking nothing
function [result]=x_dialog(labels,default_inputs_vector)
    result = default_inputs_vector;
endfunction

// Stubbing the edit_curv method
function [xx, yy, ok, gc] = edit_curv(xx, yy,  axis, args, gc)
    ok = %T;
    if ~exists("gc", "l") then
        rect=[0 0 1 1];
        axisdata=[2 10 2 10];
        gc = list(rect, axisdata);
    end
endfunction

// Stubbing the messagebox method
function [btn] = messagebox(msg, msgboxtitle, msgboxicon, buttons, ismodal)
    btn=1;
endfunction

// Stubbing global scicos flags
needcompile = 0;
alreadyran = %f;
%scicos_context = struct();

// define a function to fix model size
function model = configure(model)
    model.in = ones(model.in);
    model.in2 = ones(model.in2);
    model.intyp = ones(model.intyp);

    model.out = ones(model.out);
    model.out2 = ones(model.out2);
    model.outtyp = ones(model.outtyp);
endfunction

// If the following block simulation function does not exists then fail the
// test.
for i = 1:size(defaultlibs,"*")
    [macros, path] = libraryinfo(defaultlibs(i));
    macros=gsort(macros);
    for j = 1:size(macros,"*")
        interfunction = macros(j);

        // Not tested blocks (Xcos customs)
        if or(interfunction == notTested) then
            continue;
        end

        // Check for signature
        vars=macrovar(evstr(interfunction));
        if or([size(vars(1)) <> [3 1] , size(vars(2)) <> [3 1]]) then
            continue;
        end

        // New Scilab instance
        cmd = "scs_m=" + interfunction + "(""define"", [], []);";
        if execstr(cmd, "errcatch")<>0 then
        mprintf("%s\n",cmd); pause, end
        cmd = "scs_m=" + interfunction + "(""set"", scs_m, []);";
        if execstr(cmd, "errcatch")<>0 then
        mprintf("%s\n",cmd); pause, end

        // calling the model2blk will locate the funptr are error on
        if ~or(scs_m.model.sim == invalidFunctions) then
            model2blk(configure(scs_m.model));
        end
    end
end
clear invalidFunctions cmd vars interfunction macros path defaultlibs;
clear scs_m i j alreadyran needcompile notTested;

