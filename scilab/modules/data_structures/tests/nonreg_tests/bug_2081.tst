//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================


// <-- Non-regression test for bug 2081 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2081
//
// <-- Short Description -->
//impossible matrix insertion into hypermatrix element of a tlist


tl = tlist(["test","cube"],[]);
tl.cube = matrix([1:8], [2,2,2]);
tl.cube(:,:,1) = ones(2,2);
if or(tl.cube(:)<>[1;1;1;1;5;6;7;8]) then pause,end


