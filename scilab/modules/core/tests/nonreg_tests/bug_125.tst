//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Vincent COUVERT
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 125 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=125
//
// <-- Short Description -->
//    With one argument, size() works, but the second argument does not.

x = [1,2,3;4,5,6];

if or(size(x)<>[2,3]) then pause,end

if size(x,1)<>2 then pause,end

if size(x,2)<>3 then pause,end
