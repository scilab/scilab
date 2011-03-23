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
// Blocks must have valid dimensions for their settings.
// Some dimensions were not coherents between theirs "set" and "define" method.

loadXcosLibs();

defaultlibs = ["Branching",
               "Electrical",
               "Hydraulics",
               "Linear",
               "Sources",
               "Misc",
               "Events",
               "Integerop",
               "Matrixop",
               "Nonlinear",
               "Sinks",
               "Threshold"];

defaultlibs  = defaultlibs + "lib";
notTested = ["SUPER_f" "TEXT_f" "PAL_f", ..         // Specific blocks-
             "CBLOCK4" "CBLOCK"];                   // buggy blocks #7363

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

// If the following block display something, the test is failed.
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

        // Exported hdf5 instance
        filePath = SCI + "/modules/scicos_blocks/blocks/" + interfunction + ".h5";
        if isfile(filePath) <> %t then
            mprintf("%s not found.\n", filePath);
            continue;
        end

        if import_from_hdf5(filePath) <> %t then
            mprintf("%s not loaded\n", filePath); pause, end
        // the saved data is stored as the out variable
        scs_m = out;
        cmd = "scs_m=" + interfunction + "(""set"", scs_m, []); // loaded block";
        if execstr(cmd, "errcatch")<>0 then
            mprintf("%s\n",cmd); pause, end
    end
end

