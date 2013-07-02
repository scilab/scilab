// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- XCOS TEST -->
// <-- ENGLISH IMPOSED -->
//
// <-- Non-regression test for bug 8674 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8674
//
// <-- Short Description -->
// Check that an erroneous "From workspace" block does not produce an error.
// 

result = importXcosDiagram(SCI + "/modules/xcos/tests/nonreg_tests/bug_8674.zcos");
if (result <> %T) then pause,end

// overwrtie dialogs
prot = funcprot();
funcprot(0);

function [btn] = messagebox(msg, msgboxtitle, msgboxicon, buttons, ismodal)
    btn = 1;
endfunction

function result = dialog(labels,valueini)
    result = valueini;
endfunction

funcprot(prot);


%cpr = xcos_simulate(scs_m, 4);

