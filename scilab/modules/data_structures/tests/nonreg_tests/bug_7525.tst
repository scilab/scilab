// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 7525 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=7525
//
// <-- Short Description -->
// Scilab failed to assign 2-d matrices into 3-d matrices, on Windows.

x = zeros(2, 2, 3);
if execstr("x(:, :, 1) = [1 0; 0 1];", "errcatch") <> 0 then pause, end
