// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

//
// <-- Non-regression test for bug 6547 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6547
//
// <-- Short Description -->
// uigetdir crashed Scilab when returning some "root" path

if getos() == 'Windows' then
 r = uigetdir('c:\');
 // click on ok
 if r <> 'c:\' then pause,end
else
 r = uigetdir('/');
 // click on ok 
 if r <> '/' then pause,end
end
