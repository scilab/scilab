// ============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// ============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- Short Description -->
// Check that the simulation function should exists.

loadXcosLibs(); loadScicos();

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
notTested = ["SUPER_f", "TEXT_f", "PAL_f", ..     // Specific blocks
             "SLIDER_f", ..                       // Deprecated blocks
             "MPBLOCK", "fortran_block", ..       // blocks with code generation
             "CBLOCK", "scifunc_block", "scifunc_block_m"];

invalidFunctions = unique(gsort(["csuper", "clkfrom", "clkgoto", "delay"]));

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
        vSize = size(labelsv, '*');
        hSize = size(labelsh, '*');
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
    if ~exists("gc", 'l') then
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

BLOCKS = string([]);
AUTO_SIM = string([]);
FUNCTIONS = string([]);
FUNCTIONS_TYPES = [];

// If the following block simulation function does not exists then fail the
// test.
for i = 1:size(defaultlibs,"*")
    [macros, path] = libraryinfo(defaultlibs(i));
    for j = 1:size(macros,"*")
        interfunction = macros(j);

        // Not tested blocks (Xcos customs)
        if or(interfunction == notTested) then
          continue;
        end

        // Check for signature
        vars=macrovar(evstr(interfunction));
        if or([size(vars(1)) <> [3 1] , size(vars(2)) <> [3 1]]) then
          mprintf("%s is not a valid block descriptor.\n", interfunction);
          continue;
        end

        // New Scilab instance
        cmd = "scs_m=" + interfunction + "(""define"", [], []);";
        if execstr(cmd, "errcatch")<>0 then
            mprintf("%s\n",cmd); pause, end
        cmd = "scs_m=" + interfunction + "(""set"", scs_m, []);";
        if execstr(cmd, "errcatch")<>0 then
            mprintf("%s\n",cmd); pause, end

        // append the simulation
        sim = scs_m.model.sim;
        sim_type = [];
        if typeof(sim) == "string" then
            sim_type = 0;
            AUTO_SIM = [AUTO_SIM ; scs_m.gui];
        elseif typeof(sim) == "list" then
            sim_type = sim(2);
            sim = sim(1);
        else
            mprintf("%s has an invalid simulation type.\n", interfunction);
            continue;
        end

        BLOCKS = [BLOCKS ; scs_m.gui];
        FUNCTIONS = [FUNCTIONS ; sim];
        FUNCTIONS_TYPES = [FUNCTIONS_TYPES ; sim_type];
    end
end
clear sim sim_type cmd vars interfunction macros path defaultlibs;

// sort per function type unique functions
[FUNCTIONS, k] = unique(FUNCTIONS);
FUNCTIONS_TYPES = FUNCTIONS_TYPES(k);
BLOCKS = BLOCKS(k);
[FUNCTIONS_TYPES, k] = gsort(FUNCTIONS_TYPES);
FUNCTIONS = FUNCTIONS(k);
BLOCKS = BLOCKS(k);
clear k;

// remove invalid functions
ka = grep(FUNCTIONS, invalidFunctions);
FUNCTIONS(ka) = [];
FUNCTIONS_TYPES(ka) = [];
BLOCKS(ka) = [];

// compilation dependant blocks
if or(FUNCTIONS(find(FUNCTIONS_TYPES < 0)) <> ["ifthel";"eselect"]) then pause, end

// function implementations
sci_kind = [5 99];
f_kind = [1 3 1001 10001];
c_kind = [0 2 4 10004];

// checking scilab blocks
ka = dsearch(FUNCTIONS_TYPES, sci_kind, 'd') <> 0;
funs = FUNCTIONS(ka);
for i=1:size(funs, '*');
    if type(evstr(funs(i))) <> 13 then pause, end
end

// checking fortran blocks
ka = dsearch(FUNCTIONS_TYPES, f_kind, 'd') <> 0;
funs = FUNCTIONS(ka);
try
    link("/home/davidcl/work/branches/5.3/scilab/modules/scicos_blocks/.libs/libsciscicos_blocks.so", funs, "f");
catch
    try
        for i=1:size(funs, '*')
            l = link("/home/davidcl/work/branches/5.3/scilab/modules/scicos_blocks/.libs/libsciscicos_blocks.so", funs(i), "f");
            ulink(l);
        end
    catch
        disp("Unable to F link : " + funs(i));
    end
end

// checking C blocks
ka = dsearch(FUNCTIONS_TYPES, c_kind, 'd') <> 0;
funs = FUNCTIONS(ka);
try
    link("/home/davidcl/work/branches/5.3/scilab/modules/scicos_blocks/.libs/libsciscicos_blocks.so", funs, "c");
catch
    try
        for i=1:size(funs, '*')
            l = link("/home/davidcl/work/branches/5.3/scilab/modules/scicos_blocks/.libs/libsciscicos_blocks.so", funs(i), "c");
            ulink(l);
        end
    catch
        disp("Unable to C link : " + funs(i));
    end
end

clear funs ka;

