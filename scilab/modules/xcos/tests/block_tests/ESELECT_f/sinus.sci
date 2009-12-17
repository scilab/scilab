// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 DIGITEO Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 5660 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5660
//
// <-- Short Description -->
// This script validate the eselect functionnality with a sinus input.
// The associated diagram (sinus.xcos) is run and export data to the workspace.
// These data are then compared to the default.
// 

// Run just before the beginning of the test to load appropriate variable
// return 0: No errors
// return <> 0: Errors has occurred
function result = beforeTest()
    // Loading scicos libs
    loadScicosLibs;

    clear scs_m;
    result = importXcosDiagram(pwd() + '/sinus.xcos');

    clear A A_ref;
endfunction

// Run the test
// return 0: No errors
// return <> 0: Errors has occurred
function result = runTest()
    // will set the A variable
    xcos_simulate(scs_m);
    A = [A.values A.time];

    // open the reference variables
    A_ref = fscanfMat(pwd() + '/sinus_ref.csv');

    // Now the A and A_ref matrix contains the data
    diff_value = A - A_ref;

    // getting the maximum
    max_value = max(diff_value(:,1));

    if(max_value > 1.0D-6)
        result = 1;
    else
        result = 0;
    end
endfunction

// Run just after the end of the test
// return 0: No errors
// return <> 0: Errors has occurred
function result = afterTest()
    clear A A_ref diff scs_m;
    result = 0;
endfunction


