//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2964 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2964
//
// <-- Short Description -->
//    the function grand with option 'mul' does not accept probability one as an argument. 
y=grand(1,'mul',1,1);
if or(y<>[1;0]) then pause,end

y=grand(1,'mul',1,ones(10,1)/10);
if y($)<>0 then pause,end
if sum(y)<>1 then pause,end
