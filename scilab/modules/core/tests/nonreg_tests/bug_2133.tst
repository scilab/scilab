//<-- CLI SHELL MODE -->
// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 2133 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2133
//
// <-- Short Description -->
// parsing [/decimal number/no space/dot operator ..] can fail  
if execstr('[2.0 .*.3.0]','errcatch') <> 0 then  pause,end
if execstr('2.0 .*.3.0','errcatch') <> 0 then  pause,end
if 2.0 .*. 3.0<>6 then  pause,end
if or([2.0 .*0.3 .0]<>[0.6 0]) then pause,end
