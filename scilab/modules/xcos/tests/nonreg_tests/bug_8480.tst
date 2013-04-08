// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Clément DAVID
// Copyright (C) 2010 - Scilab Enterprises - Clément DAVID
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
//
// <-- ENGLISH IMPOSED -->
//
// <-- XCOS TEST -->
//
// <-- Non-regression test for bug 8480 -->
//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=8480
//
// <-- Short Description -->
// The debug block did not have a valid function type.
//

global Result;Result=[];

importXcosDiagram('SCI/modules/xcos/tests/nonreg_tests/bug_8480.zcos');

Info=scicos_simulate(scs_m,list());
if isempty(Result) then pause,end
clearglobal Result
