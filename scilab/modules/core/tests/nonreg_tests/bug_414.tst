//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Serge STEER
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 414 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=414
//
// <-- Short Description -->
// a tiny bug regarding the display of (boolean only?) arrays.

t=(1:35)>0;
for ll=69:73,lines(0,ll),t,end


t=(1:13);
for ll=69:6:90,lines(0,ll),t,end

t=int32(1:23);
for ll=55:2:65,lines(0,ll),t,end

