// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- JVM NOT MANDATORY -->

// <-- Non-regression test for bug 6332 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6332
//
// <-- Short Description -->
// stacksize had problems on Mac OS X and 64 bit platforms

r1 = stacksize();
stacksize('max');
mx1 = stacksize();
if r1(1) >= mx1(1) then pause, end
stacksize('max');
mx2 = stacksize();
if mx1(1) <> mx2(1) then pause, end

nmax = mx2(1) + 1e8;
ierr = execstr("stacksize(nmax);","errcatch");
if ierr == 0 then pause, end
