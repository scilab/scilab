// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 13640 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/13640
//
// <-- Short Description -->
// Graphics in isoview mode were too small when in subplots

// check that the two graphics don't seem too small

set(gcf(),"axes_size",[800 400])
x=-1:0.1:1;
subplot(1,2,1)
mesh(x,x,x'*x)
set(gca(),"isoview","on")
subplot(1,2,2)
mesh(x,x,x'*x)
set(gca(),"isoview","on")
