// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2010 Serge Steer - INRIA
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 5722 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=5722
//
// <-- Short Description -->
// tand() is buggy: tand(30) => error
ieee(0)
t1=tand(30)
ieee(2)
t2=tand(30)
if t1<>t2 then pause,end

