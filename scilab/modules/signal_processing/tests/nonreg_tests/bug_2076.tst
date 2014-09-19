// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2076 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2076
//
// <-- Short Description -->
// pspect does not use the last data window


a=[0 0 0 0 0 0 6 6];
p=pspect(2,4,"re",a,a);
if or(p<>[0 6 0 6]) then pause,end

p=pspect(2,4,"re", a);
if or(p<>[0 6 0 6]) then pause,end

