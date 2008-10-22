// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2008 - INRIA - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 3204 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3204
//
// <-- Short Description -->
// Axes box drawign disappeared in histplot demo.
// 

// first check axes
subplot(2,2,1);
histplot();
a1 = gca();

subplot(2,2,4);
histplot();
a2 = gca();

// check that the box are drawn
if (a1.box <> "on")  then pause; end
if (a2.box <> "on")  then pause; end

