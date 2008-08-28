// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge Steer <Serge.Steer@scilab.org>
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 2850 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2850
//
// <-- Short Description -->
// xpolys performance regression in Scilab 5

// should be almost as fast in Scilab 4 and Scilab 5.

clf();
u=rand(2,10000);
v=rand(2,10000);
tic();
xpolys(u,v);
toc();
