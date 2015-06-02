// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 5222 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5222
//
// <-- Short Description -->
// help command can generate a "critical exception".

for i = 1:100
    ierr = execstr("help numderivative","errcatch");
    if ierr <> 0 then pause, end
end
