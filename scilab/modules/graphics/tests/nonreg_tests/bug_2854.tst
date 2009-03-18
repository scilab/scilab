// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Jean-Baptiste Silvy
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- INTERACTIVE TEST -->

// <-- Non-regression test for bug 2854 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2854
//
// <-- Short Description -->
// plzr demo not correct
// 

s=poly(0,'s');
n=[1+s   2+3*s+4*s^2        5; 0        1-s             s];
d=[1+3*s   5-s^3           s+1;1+s     1+s+s^2      3*s-1];
h=syslin('c',n./d); 
plzr(h);

// Check that no big dots appear on the axes box corners.
// Check that X and Y labels are visible
// Check that legend is well placed


