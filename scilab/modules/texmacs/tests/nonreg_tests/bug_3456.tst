// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 3456 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=3456
//
// <-- Short Description -->
// texprint not found

s = poly(0,'s');
r = texprint([1/s,s^2]);
if r <> '{\pmatrix{{  1}\over{  s}&  s^{2}}}' then pause,end
