// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- CLI SHELL MODE -->
//
// <-- Non-regression test for bug 8820 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8820
//
// <-- Short Description -->
// squeeze function may return hypermatrix when expected results must be a regular 2D array
t = rand(2,3,4);
s = squeeze(t(:,1,:));
if type(s)<>1 then pause,end

