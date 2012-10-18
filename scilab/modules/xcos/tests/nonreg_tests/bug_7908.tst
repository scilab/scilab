// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 7908 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7908
//
// <-- Short Description -->
// scicos_getvalues did not support more than 20 arguments.

num = 30;
baseName = "arg_";

prot = funcprot();
funcprot(0);

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

//
// setup
//
values = string(1:num)';

labels = baseName + values;
execstr(labels + " = []");
argout = ", " + labels;

typ = list();
for i=1:num
	// typ update
	typ(i * 2 - 1) = "mat";
	typ(i * 2) = [1 1];
end

//
// call with num arguments
//
cmd = "[ok" + strcat(argout) + "] = scicos_getvalue(''non reg test for 7908'', labels, typ, values);";
execstr(cmd);

funcprot(prot);

