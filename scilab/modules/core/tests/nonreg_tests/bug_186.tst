//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 186 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=186
//
// <-- Short Description -->
//


a="100 100 z  ";
[values, ierr]= evstr(a);

if ierr==0 | ~isempty(values) then pause,end

a="100 100 m  ";
[values, ierr]= evstr(a);

if ierr==0 | ~isempty(values) then pause,end
