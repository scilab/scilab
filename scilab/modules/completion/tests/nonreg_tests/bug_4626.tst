// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4626 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4626
//
// <-- Short Description -->
// 
// when using the CLI version (NO GRAPHICS), the autocompletion feature make Scilab crash


// <-- CLI SHELL MODE -->
r = completion('a','graphic_properties');
if ~isdef('r') then pause,end
