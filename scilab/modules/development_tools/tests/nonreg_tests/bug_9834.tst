// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->
//

// <-- Non-regression test for bug 9834 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=9834
//
// <-- Short Description -->
// external modules was not managed by test_run


// build toolbox_skeleton
exec('SCI/contrib/toolbox_skeleton/builder.sce');

// check that tests are 'passed'
test_run('SCI/contrib/toolbox_skeleton')

