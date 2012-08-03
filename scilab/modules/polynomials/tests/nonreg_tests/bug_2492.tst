// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2006-2008 - INRIA - Serge Steer
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- CLI SHELL MODE -->

// <-- Non-regression test for bug 2492 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=2492
//
// <-- Short Description -->
// missing tests on formal variable name validity
if execstr('poly(1:3,''c+'')','errcatch')==0 then pause,end
if execstr('poly(1:3,'' c'')','errcatch')==0 then pause,end
if execstr('poly(1:3,''1c'')','errcatch')==0 then pause,end
if execstr('poly(1:3,''c*'')','errcatch')==0 then pause,end


