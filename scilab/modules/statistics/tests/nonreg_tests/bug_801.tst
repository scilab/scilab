//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA -Pierre MARECHAL <pierre.marechal@inria.fr>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 801 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=801
//
// <-- Short Description -->
//    error in the routine covar.sci

x=[4 5];
y=[3 4 5 6];
m=[ 2 3 4 5; 5 4 3 2];

if execstr('covar(x,y,m)','errcatch') <> 0 then pause,end
