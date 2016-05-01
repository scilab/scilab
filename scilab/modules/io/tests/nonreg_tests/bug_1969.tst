// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre LEDRU
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
// <-- Non-regression test for bug 1969 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=1969
//
// <-- Short Description -->
// Bad exec was causing a seg fault of Scilab

s=grand(500,1,"nor",0,1)';
a=msprintf("%2.5f ",s');

if getos() <> "Windows" then
    // cat will exit in error
    // scilab must not crash
    if unix("cat "+a+" > /dev/null 2>&1")==0 then pause,end
end
