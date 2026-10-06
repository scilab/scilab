// =============================================================================
// Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->
// <-- NO CHECK REF -->

// <-- Non-regression test for bug 3118 -->
//
// <-- GitLab URL -->
// https://gitlab.com/scilab/scilab/-/issues/3118
//
// <-- Short Description -->
// The edit/current axes properties menu of the graphic window fails
// 

if getos() <> "Darwin"
    // create a figure
    scf(0);
    
    // figure property
    ged(8,0);
    
    // axes property
    ged(9,0);
    
    // this last two commands should not fail

    // The ged widgets queue property updates. Run them here, pause being the
    // only way to flush that queue, else they run after the figure is closed.
    TCL_EvalStr("ScilabEval ""resume""");
    pause
end
