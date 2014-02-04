// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Bruno JOFRET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- INTERACTIVE TEST -->
//
// <-- Non-regression test for bug 1917 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1917
//
// <-- Short Description -->
// Scilab unexpectedly *quits* when issueing a graphical command after
// aborting the execution of a script launched from TCL

function boocrash();
    for i=1:1e6;
        TCL_EvalStr("puts {.}")
    end;
endfunction

TCL_EvalStr("ScilabEval boocrash() sync")

// Now use Ctrl-C to break scilab execution
// Try to do some graphical stuff ie plot3d
// try to abort / resume execution.

