// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- Non-regression test for bug 5686 -->
//
// <-- CLI SHELL MODE -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5686
//
// <-- Short Description -->
// Complex number manipulation could crash Scilab.

s = poly(0,'s');
tt = [ - 3.4817634, 0, 0, 0, 0, - 0.8107945, - 0.0311558, 0];
tt1 = (0*%i+[ - 3.4859813,0, 0, 0, 0, - 0.8157716, - 0.0313009, 0]);
ttt = [-.9, 10.7, 0,0,0,-1.1,-8.7,0];
a0 = (1 ./(s- tt)) * ttt';
a1 = (1 ./(s- tt1)) * ttt';
if execstr("a2 = (1 ./(s- tt1'')) * ttt;", "errcatch") <> 0 then pause, end
if execstr("a3 = (1 ./(s- tt'')) * ttt;", "errcatch") <> 0 then pause, end
if size(a2, "*") <> size(a3, "*") then pause, end
