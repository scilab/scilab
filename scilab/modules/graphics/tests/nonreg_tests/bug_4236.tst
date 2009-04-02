// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - Digiteo - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- TEST WITH GRAPHIC -->

// <-- Non-regression test for bug 4236 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4236
//
// <-- Short Description -->
// move called with an already deleted handle crashes Scilab.
// 

xpoly(1,1);
point = gce();
delete(point); // the handle is no longer valid

err = execstr("move(point, [1,1])","errcatch");
// should not crash Scilab but provide an error.
if (err == 0) then pause; end

