
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2320 -->
//
// <-- INTERACTIVE TEST -->
// UNC Path not managed

// This test is obsolete since the correction of the bug 2273 (module fileio).

if getos() == "Windows" then
    if chdir("\\Machine\folder") == %t                       then pause,end
    if execstr("cd(''\\Machine\folder'')","errcatch") <> 998 then pause,end
end

// =============================================================================
