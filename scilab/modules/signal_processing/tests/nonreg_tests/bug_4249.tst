//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA - Serge Steer 
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 4249 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=4249
//
// <-- Short Description -->
// The filter function does not work when the transfer function is a perfect pass all hz = 1/1.
Num=1;Den=1+%z;u=[1,2,3,4,5];
if or(filter(1,1,u)<>u) then pause,end

